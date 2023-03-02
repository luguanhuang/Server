#include "pch.h"
#include "ranklist.h"
#include "network/dblink.h"
#include "util/XCommon.h"
#include "pb/project.pb.h"
#include "ranklist/ptcm2d_saveranklisttodb.h"
#include "config.h"

#define PARTSIZE (1024*64)		///> 排行榜原始数据分片大小

RankDbHandler::RankDbHandler()
{
	m_emptyindb = false;
	m_finishload = false;
	m_savepolicy = NULL;
}

RankDbHandler::~RankDbHandler()
{
	if (NULL != m_savepolicy)
	{
		delete m_savepolicy;
		m_savepolicy = NULL;
	}
}

bool RankDbHandler::CheckTimeToSaveDB()
{
	if (NULL == m_savepolicy || !m_savepolicy->IsTimeToSave())
	{
		return false;
	}
	if (BeforeSaveToDB())
	{
		SendToDB();
	}
	return true;
}

void RankDbHandler::SendToDB()
{
	KKSG::RankList ranklist;
	PackDBData(&ranklist);

	LogInfo("ranktype:%u ranklist size of element:%u", m_ranklist->GetType(), m_ranklist->GetSize());

	std::string rawdata(ranklist.SerializeAsString());
	SendRawToDB(rawdata);
}

void RankDbHandler::SendRawToDB(std::string& rawdata)
{
	UINT32 ranktype = m_ranklist->GetType();
	UINT32 serverid = m_ranklist->GetServerID();
	PtcM2D_SaveRankListToDB ptc;
	ptc.m_Data.set_ranktype(ranktype);
	ptc.m_Data.set_serverid(serverid);
	ptc.m_Data.set_len(0);
	ptc.m_Data.set_index(0);
	ptc.m_Data.set_maxindex(0);

	if (rawdata.empty())
	{
		LogInfo("ranktype [%u], serverid [%u], ranklist memory size zero", ranktype, serverid);	
		ptc.m_Data.set_ranklistdata("");
		DBLink::Instance()->SendTo(ptc);
		return;
	}

	UINT32 alllen = rawdata.size();
	UINT32 count = (UINT32)(alllen/PARTSIZE);
	UINT32 left = alllen % PARTSIZE;
	if (0 == alllen % PARTSIZE)
	{
		ptc.m_Data.set_maxindex(count - 1);
	}
	else
	{
		ptc.m_Data.set_maxindex(count);
	}
	
	LogInfo("ranktype [%u], serverid [%u], ranklist memory size [%u], part[%u]", ranktype, serverid, alllen, count);

	UINT32 i = 0;
	for (; i < count; ++i)
	{
		LogInfo("ranklist part size [%u], part [%u]", PARTSIZE, i);
		ptc.m_Data.set_ranklistdata(rawdata.c_str() + i*PARTSIZE, PARTSIZE);
		ptc.m_Data.set_index(i);
		ptc.m_Data.set_len(PARTSIZE);
		ptc.SetDontCompress();
		DBLink::Instance()->SendTo(ptc);
		ptc.m_Data.clear_ranklistdata();
	}

	if (0 != left)
	{
		LogInfo("ranklist part size [%u], part [%u]", left, i);
		ptc.m_Data.set_ranklistdata(rawdata.c_str() + i*PARTSIZE, left);
		ptc.m_Data.set_index(i);
		ptc.m_Data.set_len(left);
		ptc.SetDontCompress();
		DBLink::Instance()->SendTo(ptc);
		ptc.m_Data.clear_ranklistdata();
	}
}

void RankDbHandler::ReceiveFromDB(UINT32 rankType, const KKSG::RankList& rankList)
{
	TimeMark timemark("receive ranklist from db", rankType);

	for (int i = 0; i < rankList.rankdata_size(); ++i)
	{
		const KKSG::RankData& rankdata = rankList.rankdata(i);
		UnpackDBData(rankdata, i + 1);
	}
}

/*void RankDbHandler::ReceiveFromDB(const KKSG::RankListRawData& partrawdata)
{
	UINT32 ranktype = m_ranklist->GetType();
	if (IsFinishLoad())
	{
		SSWarn<<"rank list has loaded, rank type:"<<ranktype<<END;
		return;
	}

	if (0 == partrawdata.index())
	{
		m_rawdata.clear();
	}

	m_rawdata.append(partrawdata.roleids());
	
	if (partrawdata.index() == partrawdata.maxindex())
	{
		if (0 == m_rawdata.size())
		{
			SSInfo<<"rank type:"<<ranktype<<" is empty"<<END;
			SetEmptyInDB();
		}
		else
		{
			KKSG::RankList ranklist;
			ranklist.ParseFromString(m_rawdata);

			TimeMark timemark("receive ranklist from db", ranktype);

			for (int i = 0; i < ranklist.rankdata_size(); ++i)
			{
				const KKSG::RankData& rankdata = ranklist.rankdata(i);
				UnpackDBData(rankdata, i + 1);
			}
			m_rawdata.clear();
		}
		SetFinishLoad();
		OnReceiveFromDB();
	}
}*/

/////////////////////////////////////////////////////////////////
CRankList::CRankList(UINT32 type, UINT32 serverid)
{
	m_type = type;
	if (serverid == 0) m_serverid = MSConfig::Instance()->GetServerID();
	else m_serverid = serverid;
	m_timestamp = 0;
	m_db = NULL;
}

CRankList::~CRankList()
{
	if (NULL != m_db)
	{
		delete m_db;
		m_db = NULL;
	}
}

void CRankList::RegisterDb(RankDbHandler* db) 
{ 
	m_db = db;
	m_db->Init(this);
}
