#include "pch.h"
#include "time.h"
#include "db/task/ranklistsavetask.h"
#include "db/mysqlmgr.h"
#include "ranklist/ptcm2d_saveranklisttodb.h"

// generate by ProtoGen at date: 2016/8/21 19:16:56

void UpdateRankData(std::string& strRawRankData, KKSG::RankListRawData& oData)
{
	///> 开始组包
	if (0 == oData.index())
	{
		strRawRankData.clear();
	}
	///>　组包
	strRawRankData.append(oData.ranklistdata());
	///> 组包结束
	if (oData.index() == oData.maxindex())
	{
		RankListSaveTask* poTask = new RankListSaveTask();
		poTask->m_dwType = oData.ranktype();
		poTask->m_dwServerID = oData.serverid();
		poTask->m_strRankData = strRawRankData;
		CMysqlMgr::Instance()->PushTask(poTask);

		SSInfo<<"run rank list update task, ranktype:"<<oData.ranktype()<<", serverid:"<<oData.serverid()<<END;
	}
}

void PtcM2D_SaveRankListToDB::Process(UINT32 dwConnID)
{
	static std::map<std::pair<UINT32, UINT32>, std::string> strRawRankDatas;

	UINT32 rankType = m_Data.ranktype();
	UINT32 serverid = m_Data.serverid();
	SSInfo<<"start ranktype:"<<rankType<<" serverid:"<<serverid<<" size:"<<m_Data.len()<<" index:"<<m_Data.index()<<" maxindex:"<<m_Data.maxindex()<<END;

	UpdateRankData(strRawRankDatas[std::make_pair(rankType, serverid)], m_Data);
}
