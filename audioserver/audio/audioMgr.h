#ifndef __AUDIO_MGR_H__
#define __AUDIO_MGR_H__

#include "pb/project.pb.h"
#include "pch.h"
#include <map>
#include <string>
#include "define.h"
#include "singleton.h"

#include "api_log.h"
#include "api_log_demo.h"
#include "api_get_authkey.h"
#include "api_stt.h"


struct stAudioInfo
{
	UINT64 audioUid;
	std::string audio;
	std::string text;
	UINT32 time;
};


class VoipInterface;
class VoipEvent;


union UID
{
	UINT64 id;
	struct {
		UINT16 index;
		UINT16 tag;	//type:4 serverid:12
		UINT32 timestamp;
	} _;
};



class AudioMgr
{
	AudioMgr();
	~AudioMgr();


	DECLARE_SINGLETON(AudioMgr);
public:
#ifdef WIN32
	void* LoadVoiceLibrary(const char *dll,const char *fname);
#endif
	bool Init();
	void ReadGrayConfig();
	bool InitAudioText();
	void Uninit();
	void Update();
	void WriteAudioIndexFile( );
	void ReadAudioIndexFile( );
	void AddAudio(const std::string &audio, const std::string &text, UINT64& uid,bool bCross=false);
	UINT64 CreateUID(bool bCross);
	UINT32 GetSvrID(UINT64 dwID);

	void SendAuthKey(const std::string& ip, const std::string &open_id,UINT32 params);
	void Translate(std::string fileid,UINT32 nRpcID);
	void LoadVoipXML();
	stAudioInfo* GetAudio(UINT64 uid);
	size_t FileSize();

	void WriteFile();
	void WriteOneFile( KKSG::AudioInfo& audio );
	void ReadOneFile( UINT32 time, UINT64 uid );
	void DeleteOneFile( UINT32 time, UINT64 uid );

	///>获取时间前缀：格式 2016-05-01_12-30-30
	std::string GetTimePrefix( UINT32 time );
	std::string GenerateFileName( UINT32 time, UINT64 audioUid );
	
	UINT32 GetGayFlag();
public:
	std::map<UINT64, stAudioInfo> m_mapAudio;
	FILE* m_pFile;

	FILE *m_pLogFile;
	UINT32 m_dwLastWriteTime;
	INT32 m_dwMaxAudioCount; 
	INT32 m_dwOneAudioSize;
private:
	char *m_pFileCache;
	char *m_pFileIndex;
	UINT32 m_nRecord;
	UINT32 m_nGray;
	UINT32 m_nGrayTime;
	UINT32 m_nTime;


	ns_ga::get_authkey_api_interface* m_pPga;
	ns_stt::stt_api_interface* m_pStt; 
	ns_log::MLogInterface* m_pMLog;
	ns_log::MLog *m_pSttLog;

	std::vector<std::string> m_vecVoipSvrURL;
	std::string m_strBusinessName;
	std::string m_strBusinessKey;
	std::string m_strWindowsDllName;
	std::string m_strLinuxAName;
	UINT32 m_nAudioID;
};

#endif