#include "../../protocol/pb/project.pb.h"
#include "pch.h"
#include "LogInit.h"
#include "audioMgr.h"
#include "timeutil.h"
#include "tinyxml2.h"
#include "util.h"
#include "crpc.h"
#include "config.h"

#ifdef WIN32
#include <io.h>

typedef ns_ga::get_authkey_api_interface* __stdcall create_get_authkey_instance_t(ns_ga::get_authkey_event_handler*,ns_log::MLogInterface*);
create_get_authkey_instance_t *create_authkey_instance_p;

typedef ns_stt::stt_api_interface* __stdcall create_stt_instance_t(ns_stt::stt_event_handler*,ns_log::MLogInterface*);
create_stt_instance_t *create_stt_instance_p;

#endif




INSTANCE_SINGLETON(AudioMgr);

///>最大语音UID
static const UINT64 max_audio_uid = 0xFFFFFFFF - 5; 

///>写文件时间间隔: 5分钟
static const UINT32 write_file_interval = 300;

static const std::string tablePath					= "audioconf/table/";
static const std::string audioFilePath				= "audioconf/audio_file/";
static const std::string audioIndexFileName	= "audioindex.txt";

///>每个语音文件最大体积
static const UINT32 max_one_audio_file_size = 30000;

///>语音索引文件最大体积: 2M    5W * 8 = 40W byte = 400K, 此处分大一点
static const UINT32 max_audio_index_file_size = 20480000;



class GetAuthkeyEvent:public ns_ga::get_authkey_event_handler
{
public:
	virtual bool	on_get_authkey_return(const ns_ga::GetAuthkeyResult*pr)
	{
		if(pr)
		{
			KKSG::AudioAuthKeyRes res;
			res.set_dwmainsvrurl1(inet_addr(pr->dwMainSvrUrl1));
			res.set_dwmainsvrurl2(inet_addr(pr->dwMainSvrUrl2));
			res.set_dwmainsvrid(pr->dwMainSvrId);
			res.set_dwslavesvrid(pr->dwSlaveSvrId);
			res.set_dwslavesvrurl1(inet_addr(pr->dwSlaveSvrUrl1));
			res.set_dwslavesvrurl2(inet_addr(pr->dwSlaveSvrUrl2));
			res.set_dwexpirein(pr->dwExpireIn);
			res.set_szauthkey(pr->szAuthKey);
			CUserData data;
			data.m_dwUserData = 0;
			data.m_pUserPtr   = &res;
			CRpc::ReplyDelayRpc(atoi(pr->async_data), data);
			LogDebug("---dwExpireIn=%d dwMainSvrId=%d dwMainSvrUrl1=%s dwMainSvrUrl2=%s szAuthKey=%s dwExpireIn=%d async_data=%s",
				pr->dwExpireIn,pr->dwMainSvrId,pr->dwMainSvrUrl1,pr->dwMainSvrUrl2, pr->szAuthKey,pr->dwExpireIn,pr->async_data);
		}

		return true;
	}

	virtual bool	on_get_authkey_error(const ns_ga::GetAuthkeyError*pr)
	{
		if(pr)
		{
			KKSG::AudioAuthKeyRes res;
			res.set_error(KKSG::ERR_AUDIOTXT);
			CUserData data;
			data.m_dwUserData = 0;
			data.m_pUserPtr   = &res;
			CRpc::ReplyDelayRpc(atoi(pr->async_data), data);

		}
		return true;
	}
};



class SttEvent:public ns_stt::stt_event_handler
{
public:
	virtual bool	on_stt_return(const ns_stt::SttResult *pr)
	{
		if(pr)
		{
			KKSG::AudioTextRes  res;
			res.set_file_text(pr->file_text);
			CUserData data;
			data.m_dwUserData = 0;
			data.m_pUserPtr   = &res;
			CRpc::ReplyDelayRpc(atoi(pr->async_data), data);
			SSInfo << "async" << pr->async_data << "async_len" << pr->async_len << "file_id" << pr->file_id << "file_text" << pr->file_text << END;
		}
		return true;
	}

	virtual bool	on_stt_error(const ns_stt::SttError*pr)
	{
		if( pr)
		{
			KKSG::AudioTextRes  res;
			res.set_error(KKSG::ERR_AUDIOTXT);
			CUserData data;
			data.m_dwUserData = 0;
			data.m_pUserPtr   = &res;
			CRpc::ReplyDelayRpc(atoi(pr->async_data), data);
			SSError << "stterror async_data" << pr->async_data << "async_len" << pr->async_len << "error_code" << pr->error_code << "pr->error_desc" << pr->error_desc << END;
		}
		return true;
	}
};




AudioMgr::AudioMgr()
{
	m_dwLastWriteTime	= 0; 
	m_pFile				= NULL; 
	m_pFileCache		= NULL;
	m_pFileIndex		= NULL;
	m_pLogFile			= NULL;
	m_pPga				= NULL;
	m_pMLog				= NULL;
	m_pSttLog			= NULL;
	m_nGray				= 0;
	m_nGrayTime			= 0;
	m_nAudioID          = 0;
	m_nTime				= 0;
}


AudioMgr::~AudioMgr()
{
	if (m_pFile)
	{
		fclose(m_pFile);
		m_pFile = NULL; 
	}
	if (m_pFileCache)
	{
		delete []m_pFileCache;
		m_pFileCache = NULL;
	}
	if (m_pFileIndex)
	{
		delete []m_pFileIndex;
		m_pFileIndex = NULL;
	}
	if (m_pPga)
	{
		delete m_pPga;
		m_pPga = NULL;
	}
	if (m_pMLog)
	{
		delete m_pMLog;
		m_pMLog = NULL;
	}
	if(m_pSttLog)
	{
		delete m_pSttLog;
		m_pSttLog = NULL;
	}

}



#ifdef WIN32
void* AudioMgr::LoadVoiceLibrary(const char *dll,const char *fname)
{
	HINSTANCE hGetProcIDDLL = LoadLibrary("voip.dll");
	if (!hGetProcIDDLL) 
	{
		UINT32 errorcode = GetLastError();
		return NULL;
	}
	void* funci = GetProcAddress(hGetProcIDDLL, fname);
	if (!funci) 
	{
		return NULL;
	}
	LogDebug("LoadLibrary[%s=>%s]=(%p)",dll,fname,funci);
	return funci;
}
#endif


bool AudioMgr::InitAudioText()
{
	static GetAuthkeyEvent event_imp;
	static SttEvent sstevent_imp;
	m_pMLog = new ns_log::MLog();
	m_pSttLog =  new ns_log::MLog();
	std::string dllname = "voip.dll";
#ifdef WIN32
	create_authkey_instance_p = (create_get_authkey_instance_t *)LoadVoiceLibrary(dllname.c_str(), "create_get_authkey_instance");
	create_stt_instance_p = (create_stt_instance_t *)LoadVoiceLibrary(dllname.c_str(), "create_stt_instance");
	m_pPga = create_authkey_instance_p(&event_imp,m_pMLog);
	m_pStt = create_stt_instance_p(&sstevent_imp, m_pSttLog);
#else
	m_pPga =  create_get_authkey_instance(&event_imp,m_pMLog);
	m_pStt = create_stt_instance(&sstevent_imp, m_pSttLog);
#endif

	if(!m_pPga)
	{
		LogError("create_get_authkey_instance error");
		return false;
	}
	if (!m_pSttLog)
	{
		LogError("create_stt_instance error");
		return false;
	}

	ns_ga::GetAuthkeyConfig config;
	if (AudioConfig::Instance()->GetDebugEnv())
	{
		LogDebug("read test env conf");
		strncpy(config.dir_svr_url[0],  "udp://101.226.103.77:10006",  strlen("udp://101.226.103.77:10006")+1);
		config.dir_svr_url_count++; 
	}else
	{
		for (UINT32 i = 0; i < m_vecVoipSvrURL.size() && i<3; i++)
		{
			strncpy(config.dir_svr_url[i],  m_vecVoipSvrURL[i].c_str(),  m_vecVoipSvrURL[i].length()+1);
			config.dir_svr_url_count++; 
		}
	}

	strncpy(config.business_name, m_strBusinessName.c_str(), m_strBusinessName.length()+1);
	strncpy(config.business_key, m_strBusinessKey.c_str(), m_strBusinessKey.length()+1);
	if(!m_pPga->init(config))
	{
		SSError<<"init  m_pPga  error"<<END;
		return false;
	}

	ns_stt::SttConfig sstconfig;
	for (UINT32 i = 0; i < m_vecVoipSvrURL.size() && i<3; i++)
	{
		strncpy(sstconfig.dir_svr_url[i],  m_vecVoipSvrURL[i].c_str(),  m_vecVoipSvrURL[i].length()+1);
		sstconfig.dir_svr_url_count++; 
	}
	strncpy(sstconfig.business_name, m_strBusinessName.c_str(), m_strBusinessName.length()+1);
	strncpy(sstconfig.business_key, m_strBusinessKey.c_str(), m_strBusinessKey.length()+1);
	if (!m_pStt->init(sstconfig))
	{
		SSError<<"init  m_pStt  error"<<END;
		return false;
	}
	return true;
}




//文件格式  UID  音频  文本;
bool AudioMgr::Init()
{ 
	LoadVoipXML();
	InitAudioText();

	m_mapAudio.clear();
	m_dwMaxAudioCount = 0;
	m_dwOneAudioSize = 0;
	m_nRecord = 0;

	//配置文件路径;
	std::string audioParamFile = tablePath + "audioparam.xml";

	tinyxml2::XMLDocument doc;
	tinyxml2::XMLError error = doc.LoadFile( audioParamFile.c_str() );
	if (error != tinyxml2::XML_SUCCESS)
	{
		LogError("Parser AudioParam File %s get XMLError code: %d", audioParamFile.c_str(), error);
		return false;
	}
	tinyxml2::XMLElement* rootElem = doc.RootElement();
	if( NULL == rootElem )
	{
		LogError("XML Root Null, FileName: %s", audioParamFile.c_str() );
		return false; 
	}
	tinyxml2::XMLElement *paramvalue = rootElem->FirstChildElement("ParamValue");  
	if( NULL == paramvalue )
	{
		LogError("Param Value Node Null, FileName: %s", audioParamFile.c_str() );
		return false; 
	}	
	paramvalue->QueryIntAttribute("one_audio_size", &m_dwOneAudioSize);
	paramvalue->QueryIntAttribute("max_audio_count", &m_dwMaxAudioCount);
	LogWarn("Load XML Success, OneAudioSize:%d, MaxAudioCount:%d", m_dwOneAudioSize, m_dwMaxAudioCount);
	m_pFileIndex = new char[max_audio_index_file_size];
	m_pFileCache = new char[m_dwOneAudioSize];
	///>加载语音索引文件：保存语音信息的UID和time
	ReadAudioIndexFile();
	SSWarn << "Init Audio Success" << END;
	std::string szLogFile = tablePath + "xunfei.log";
	m_pLogFile= fopen(szLogFile.c_str(), "a+");
	if (!m_pLogFile)
	{
		return false;
	}
	ReadGrayConfig();
	return true;
}

void AudioMgr::ReadGrayConfig()
{
	std::string strFilePath =tablePath + "gray.txt";
	std::ifstream infile(strFilePath.c_str());
	std::string strVal;
	if (!std::getline(infile, strVal))
	{
		LogError("Read Gay failed, file: %s", strFilePath.c_str());
		return;
	}
	std::string temp = Trim(strVal);
	m_nGray = atoi(temp.c_str());
	infile.close();
	LogInfo("ReadGrayConfig=%d",m_nGray);
}

void AudioMgr::Uninit()
{
	SSWarn << "Uninit Instance" << END;
	WriteAudioIndexFile();
	if (m_pLogFile)
	{
		char szBuff[64] = {0};
		UINT32 nLen = sprintf(szBuff,"time=%s record=%d\n",TimeUtil::GetDateTimeStr(m_nTime).c_str(), m_nRecord) ;
		fwrite(szBuff, nLen,1,m_pLogFile);
		fflush(m_pLogFile);
		fclose(m_pLogFile);
	}

}

//每10分钟保存一次数据
void AudioMgr::Update()
{
	m_pPga->update();
	m_pStt->update();
	m_nTime = TimeUtil::GetTime();

	if (m_nTime > m_nGrayTime)
	{
		ReadGrayConfig();
		m_nGrayTime = m_nTime+60;
	}

	if( m_dwLastWriteTime + write_file_interval > m_nTime )
	{
		return; 
	}
	///>定时写索引文件
	WriteAudioIndexFile();
	m_dwLastWriteTime = m_nTime; 
}

size_t AudioMgr::FileSize()
{
	if( NULL == m_pFile )
	{
		return 0; 
	}
	fseek(m_pFile, 0L, SEEK_END);
	size_t sz = ftell(m_pFile);
	return sz; 
}


void AudioMgr::SendAuthKey(const std::string& ip,const std::string & open_id,UINT32  nRpcID)
{
	ns_ga::GetAuthkeyPara para;
	strncpy(para.client_ip,ip.c_str(),sizeof(para.client_ip));
	strncpy(para.open_id, open_id.c_str(),sizeof(para.open_id));
	para.async_len=snprintf(para.async_data,sizeof(para.async_data),"%d",nRpcID);
	if(!m_pPga->send_get_authkey_req(&para))
	{
		SSError << "IP" << ip << " open_id"  << open_id << " nRpcID" << nRpcID << END;
	}
	SSInfo << "IP" << ip << " open_id"  << open_id << " nRpcID" << nRpcID << END;

}



void AudioMgr::Translate(std::string fileid,UINT32 nRpcID)
{
	ns_stt::SttPara para;
	strncpy(para.file_id,fileid.c_str(),sizeof(para.file_id));
	para.async_len=snprintf(para.async_data,sizeof(para.async_data),"%d",nRpcID);
	m_pStt->send_stt_req(&para);
}



///>加载语音xml配置文件
void AudioMgr::LoadVoipXML()
{
	std::string voipFilePath = "gsconf/serveronly/";
	std::string voipParamFile = voipFilePath + "voipparam.xml";
	tinyxml2::XMLDocument doc;
	tinyxml2::XMLError error = doc.LoadFile(voipParamFile.c_str());
	if (error != tinyxml2::XML_SUCCESS)
	{
		LogError("Parser Voip Param File %s get XMLError code: %d", voipParamFile.c_str(), error);
		return;
	}
	tinyxml2::XMLElement* rootElem = doc.RootElement();
	if(NULL == rootElem)
	{
		LogError("XML Root Null, FileName: %s", voipParamFile.c_str());
		return; 
	}
	tinyxml2::XMLElement *paramvalue = rootElem->FirstChildElement("ParamValue");  
	if(NULL == paramvalue)
	{
		LogError("Param Value Node Null, FileName: %s", voipParamFile.c_str());
		return; 
	}	


	m_vecVoipSvrURL = Split(paramvalue->Attribute("dir_svr_url"), '|');
	m_strBusinessName = paramvalue->Attribute("business_name");
	m_strBusinessKey = paramvalue->Attribute("business_key");

	tinyxml2::XMLElement* libNode = rootElem->FirstChildElement("LibName");
	if(NULL == libNode)
	{
		LogError("LibName Node Null, FileName: %s", voipParamFile.c_str());
		return; 
	}
	m_strWindowsDllName = libNode->Attribute("windowsdll");
	m_strLinuxAName = libNode->Attribute("linuxa");
	return; 
}


void AudioMgr::AddAudio(const std::string &audio, const std::string &text, UINT64& uid,bool bCross)
{
	stAudioInfo stc;
	stc.audioUid = CreateUID(bCross);
	stc.audio = audio;
	stc.text = text;
	stc.time = m_nTime;
	m_mapAudio.insert(std::make_pair(stc.audioUid, stc));
	uid = stc.audioUid;
	char szBuff[256] = {0};
	m_nRecord++;
	//最大不超过5000条
	while( (INT32)m_mapAudio.size() > m_dwMaxAudioCount )
	{
		UINT64 innerAudioUid = m_mapAudio.begin()->second.audioUid;
		UINT32 innerAudioTime = m_mapAudio.begin()->second.time;
		m_mapAudio.erase( m_mapAudio.begin() );
		DeleteOneFile( innerAudioTime, innerAudioUid );
	}

	///>实时写文件：每个语音写一个文件: 测试写一个文件耗时为500-700微秒
	UINT64 beginMicroSec = TimeUtil::GetMicroSecond();
	KKSG::AudioInfo writeaudio;
	writeaudio.set_uid(stc.audioUid);
	writeaudio.set_time(stc.time);
	writeaudio.set_audio(stc.audio);
	writeaudio.set_text(stc.text);
	WriteOneFile(writeaudio);
	UINT64 endMicroSec = TimeUtil::GetMicroSecond();
	UINT32 useMicroSec = (UINT32)(endMicroSec - beginMicroSec);
	SSInfo << "Write One File Use Micro Second: " << useMicroSec << END;

}

UINT64 AudioMgr::CreateUID(bool bCross)
{
	UID uid;
	uid._.timestamp = m_nTime;
	uid._.index		= m_nAudioID++;
	if (bCross == true)
		uid._.tag		= AudioConfig::Instance()->GetServerID();
	else
		uid._.tag		= 0;
	return uid.id;
}

UINT32 AudioMgr::GetSvrID(UINT64 dwID)
{
	UID uid;
	uid.id = dwID;
	return uid._.tag;
}

stAudioInfo* AudioMgr::GetAudio(UINT64 uid)
{
	std::map<UINT64, stAudioInfo>::iterator itFind = m_mapAudio.find(uid);
	if( itFind == m_mapAudio.end() )
	{
		return NULL;
	}
	return &(itFind->second);
}

void AudioMgr::WriteFile()
{
	KKSG::AudioList audiodata; 
	for( auto i=m_mapAudio.begin(); i!= m_mapAudio.end(); i++ )
	{
		KKSG::AudioInfo* pData = audiodata.add_datalist();
		pData->set_uid( i->second.audioUid );
		pData->set_audio( i->second.audio );
		pData->set_text( i->second.text );
	}

	std::string packdata;
	audiodata.SerializeToString(&packdata);

	SSInfo << "Add Audio Success, " << END;

#ifndef WIN32 
	ftruncate(fileno(m_pFile), 0);
#else
	_chsize(_fileno(m_pFile), 0);
#endif

	size_t sz = FileSize();
	if( 0 != sz )
	{
		SSError << "File Size Not Zero Before Write!!!" << END;
	}

	rewind(m_pFile);
	fwrite(packdata.c_str(), packdata.size(), 1, m_pFile);
	fflush(m_pFile);
	sz = FileSize();
	SSInfo << "Finish Write File, Audio Count:" << m_mapAudio.size() << ", AudioFileSize:" << sz << END;
}


void AudioMgr::WriteOneFile(KKSG::AudioInfo& audio)
{
	std::string strserialize = "";
	audio.SerializeToString(&strserialize);
	std::string timePrefix = GetTimePrefix( audio.time() );
	std::string fileName = audioFilePath + timePrefix + "." + ToString( audio.uid() ) + ".txt";
	FILE* pFile = fopen( fileName.c_str(), "ab" );
	if( NULL == pFile )
	{
		SSError << "open file failed, filename:" << fileName.c_str() << END;
		return; 
	}

///>清空文件
#ifndef WIN32 
	ftruncate(fileno(pFile), 0);
#else
	_chsize(_fileno(pFile), 0);
#endif

	rewind(pFile);
	fwrite( strserialize.c_str(), strserialize.size(), 1, pFile );
	fflush(pFile);
	fclose(pFile);
}

void AudioMgr::ReadOneFile( UINT32 time, UINT64 uid )
{
	///>文件名：2016-05-01_14-30-00.audiouid.txt
	std::string filename = audioFilePath + GetTimePrefix(time) + "." + ToString(uid) + ".txt";
	memset(m_pFileCache, 0, m_dwOneAudioSize);
	FILE* pFile = fopen( filename.c_str(), "rb" );
	if( NULL == pFile )
	{
		SSError << "Open File Failed, Maybe Not Exist, FileName:" << filename << END;
		return; 
	}
	UINT32 readNum = fread(m_pFileCache, 1, m_dwOneAudioSize,  pFile);
	fclose(pFile);
	std::string strcontent = std::string(m_pFileCache, readNum);
	KKSG::AudioInfo audio;
	if( !audio.ParseFromString(strcontent) )
	{
		SSError << "Parse Audio File Failed, Time:" << time << ", UID:" << uid << ", FileName:" << filename << ", readNum:" << readNum << ", FileLen:" << strcontent.size() << END;
		return; 
	}
	SSInfo << "Parse Audio File Success, Time:" << time << ", UID:" << uid << ", FileName:" << filename << ", readNum:" << readNum << ", Time:" << audio.time() << ", Uid:" << audio.uid() << ", FileLen:" << strcontent.size() << END;
	stAudioInfo stc;
	stc.audioUid = audio.uid();
	stc.audio = audio.audio();
	stc.text = audio.text();
	stc.time = audio.time();
	m_mapAudio.insert( std::make_pair( stc.audioUid, stc) );
}

void AudioMgr::DeleteOneFile( UINT32 time, UINT64 uid )
{
	std::string fileName = GenerateFileName(time, uid);
	INT32 result = remove(fileName.c_str());
	if( 0 != result )
	{
		SSError << "Delete Audio File Failed, FileName:" << fileName << ", AudioCount:" << m_mapAudio.size() << END;
		return; 
	}
	SSInfo << "Delete Audio File Success, FileName:" << fileName << ", AudioCount:" << m_mapAudio.size() << END;
}


void AudioMgr::WriteAudioIndexFile()
{
	KKSG::AudioList alist;
	for( auto i=m_mapAudio.begin(); i != m_mapAudio.end(); i++ )
	{
		KKSG::AudioInfo* audioIndex = alist.add_datalist();
		audioIndex->set_time( i->second.time );
		audioIndex->set_uid( i->second.audioUid );
	}	
	SSInfo << "Write Audio Index File, Audio Count:" << m_mapAudio.size() << END;
	std::string strcontent = "";
	if( !alist.SerializeToString(&strcontent) )
	{
		SSError << "Serialize Audio Index Failed, Index Count:" << alist.datalist_size() << END;
		return; 
	}
	SSInfo << "Write Audio Index File, Audio Count:" << m_mapAudio.size() << ", FileSize:" << strcontent.size() << END;
	std::string fileName = audioFilePath + audioIndexFileName;
	FILE* pFile = fopen( fileName.c_str(), "ab" );
	if( NULL == pFile )
	{
		SSError << "Open Audio Index File Failed, FileName:" << fileName << END;
		return; 
	}

///>清空文件
#ifndef WIN32 
	ftruncate(fileno(pFile), 0);
#else
	_chsize(_fileno(pFile), 0);
#endif

	rewind( pFile );
	fwrite( strcontent.c_str(), strcontent.size(), 1, pFile );
	fflush( pFile );
	fclose( pFile );
	SSInfo << "Finish Write Audio Index File, Audio Count:" << m_mapAudio.size() << ", FileSize:" << strcontent.size() << END;
}

void AudioMgr::ReadAudioIndexFile( )
{
	std::string fileName = audioFilePath + audioIndexFileName;
	FILE* pFile = fopen( fileName.c_str(), "rb" );
	if( NULL == pFile )
	{
		SSError << "Open Audio Index File Failed, FileName:" << fileName << END;
		return; 
	}
	memset(m_pFileIndex, 0, max_audio_index_file_size);
	UINT32 readNum = fread(m_pFileIndex, 1, max_audio_index_file_size,  pFile);
	fclose(pFile);
	std::string strcontent = std::string(m_pFileIndex, readNum );
	KKSG::AudioList alist;
	if( !alist.ParseFromString(strcontent) )
	{
		SSError << "Parse Audio Index File Failed, FileSize:" << readNum << END;
		return; 
	}
	UINT32 count = alist.datalist_size();
	for( INT32 i=0; i<alist.datalist_size(); i++ )
	{
		UINT32 time = alist.datalist(i).time();
		UINT64 uid = alist.datalist(i).uid();
		ReadOneFile( time, uid );
	}
	SSWarn << "Read Audio Index File Success, Audio Count: " << alist.datalist_size() << END;
}

///>获取时间前缀：格式 20160501_12-30-30
std::string AudioMgr::GetTimePrefix( UINT32 time )
{
	std::string strDateTime = TimeUtil::GetDateTimeStr( time );
	std::string year =  strDateTime.substr (0, 4); 
	std::string mon = strDateTime.substr(5, 2);
	std::string day = strDateTime.substr(8, 2);
	std::string hour = strDateTime.substr(11, 2);
	std::string min = strDateTime.substr(14, 2);
	std::string sec = strDateTime.substr(17, 2);
	std::string tmp = TimeUtil::GetDateStr(time);
	std::string dateTime = TimeUtil::GetDateStr(time) + "_" + hour + "-" + min + "-" + sec;
	return dateTime;
}

std::string AudioMgr::GenerateFileName( UINT32 time, UINT64 audioUid )
{
	std::string fileName = audioFilePath + GetTimePrefix(time) + "." + ToString(audioUid) + ".txt";
	return fileName;
}

UINT32 AudioMgr::GetGayFlag()
{
	return m_nGray;
}
