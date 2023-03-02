#include "pch.h"
#include "levelsealMgr.h"
#include "global/GlobalConfigDBBuild.h"
#include "notice/noticemgr.h"
#include "util.h"
#include <time.h>
#include "util/XCommon.h"
#include "config.h"
#include "timeutil.h"
#include "levelseal/ptcm2g_mslevelsealntf.h"
#include "network/gslink.h"
#include "mail/mailmgr.h"
#include "table/globalconfig.h"
#include "push/pushmgr.h"
#include "nestweek/nestweekmgr.h"
#include "util/gametime.h"
#include "other/servercrossdatamgr.h"

INSTANCE_SINGLETON(CLevelSealMgr)

CLevelSealMgr::CLevelSealMgr()
{
	m_bStartServerSave = false;
}
CLevelSealMgr::~CLevelSealMgr()
{

}
bool CLevelSealMgr::Init()
{
	if (!LoadFile())
	{
		return false;
	}
	InitTypeInfoFromDB();//先加载配置，然后根据开服时间和配置计算当前封印;
	m_TimeHandler = CTimerMgr::Instance()->SetTimer(this, 0, 3000, -1, __FILE__, __LINE__);
	return true;
}
void CLevelSealMgr::Uninit()
{
	CTimerMgr::Instance()->KillTimer(m_TimeHandler);
}
void CLevelSealMgr::NotifyLevelSealInfo(bool isGmSet)
{
	PtcM2G_MsLevelSealNtf ntf;//定时同步数据到所有的gs;
	ntf.m_Data.set_type(m_SealInfo.m_uType);
	ntf.m_Data.set_level(m_SealInfo.m_uLevel);
	ntf.m_Data.set_starttime(m_SealInfo.m_uStartTime);
	ntf.m_Data.set_endtime(m_SealInfo.m_uEndTime);
	ntf.m_Data.set_isgmnotify(isGmSet);
	ntf.m_Data.set_totalcollectcount(m_SealInfo.m_uTotalCollectCount);
	ntf.m_Data.set_updatetime(UINT32(GameTime::GetTime()));
	for (auto it = m_vUnlockInfo.begin(); it != m_vUnlockInfo.end(); it ++)
	{
		ntf.m_Data.mutable_unlockinfo()->add_type(it->first);
		ntf.m_Data.mutable_unlockinfo()->add_unlocktime(it->second);
	}
	GSLink::Instance()->SendToAllLine(ntf);
}
void CLevelSealMgr::OnTimer(UINT32 dwID, UINT32 dwCount)
{
	CheckLevelSealInfo();//定时检查;
	NotifyLevelSealInfo();
}

const LevelSealTypeTable::RowData* CLevelSealMgr::GetLevelSealTypeData(UINT32 uType)
{
	LevelSealTypeTable::RowData* pRow = m_oTypeTable.GetByType(uType);
	return pRow;
}

bool CLevelSealMgr::CheckFile()
{
	LevelSealTypeTable tmpTypeTable;
	if (!tmpTypeTable.LoadFile("table/LevelSealType.txt"))
	{
		SSWarn<<"load file table/LevelSealType.txt"<<END;
		return false;
	}
	for (auto i = tmpTypeTable.Table.begin(); i != tmpTypeTable.Table.end(); i ++)
	{
		if (!(*i)->Type)
		{
			LogWarn("invalid levelseal type config");
			return false;
		}
	}

	TeamCostScale teamcosttable;
	if (!teamcosttable.LoadFile("table/TeamCostScale.txt"))
	{
		SSWarn<<"load file table/TeamCostScale.txt failed"<<END;
		return false;
	}
	return true;
}

void CLevelSealMgr::ClearFile()
{
	m_oTypeTable.Clear();
	m_teamcosttable.Clear();
}
bool CLevelSealMgr::LoadFile()
{
	if (!CheckFile())
	{
		return false;
	}
	ClearFile();
	m_oTypeTable.LoadFile("table/LevelSealType.txt");
	for (auto i = m_oTypeTable.Table.begin(); i != m_oTypeTable.Table.end(); i ++)
	{
		LogInfo("config type=%u,level=%u,time=%u", (*i)->Type, (*i)->Level, (*i)->Time);
	}
	InitTypeInfoFromCfg(m_SealInfo.m_uType);

	m_teamcosttable.LoadFile("table/TeamCostScale.txt");

	return true;
}
void CLevelSealMgr::InitTypeInfoFromDB()
{
	UINT32 uType = convert<UINT32>(CGlobalConfigDBBuild::Instance()->GetConfigValue(eLevelSealType));
	UINT32 uStartTime = convert<UINT32>(CGlobalConfigDBBuild::Instance()->GetConfigValue(eLevelSealStartTime));
	UINT32 uTotalCollectCount = convert<UINT32>(CGlobalConfigDBBuild::Instance()->GetConfigValue(eLevelSealCollectCount));
	std::string strUnlockInfo = CGlobalConfigDBBuild::Instance()->GetConfigValue(eLevelSealUnLockTime);
	m_SealInfo.m_uType = uType;
	m_SealInfo.m_uStartTime = uStartTime;
	m_SealInfo.m_uTotalCollectCount = uTotalCollectCount;
	KKSG::LevelSealUnlockInfo roInfo;
	if (roInfo.ParseFromString(strUnlockInfo))
	{
		if (roInfo.type_size() == roInfo.unlocktime_size())
		{
			for (INT32 i = 0; i < roInfo.type_size(); i ++)
			{
				m_vUnlockInfo.insert(std::make_pair(roInfo.type(i), roInfo.unlocktime(i)));
			}
		}
	}
	if (m_SealInfo.m_uStartTime == 0 || (uType == 1 && uStartTime != MSConfig::Instance()->GetGameServerOpenTimeStamp()))//从来还没有初始化过封印、开服时间已修改
	{
		m_SealInfo.m_uStartTime = MSConfig::Instance()->GetGameServerOpenTimeStamp();
	}

	//起服的时候直接根据开服时间算一遍，不使用存储时间;
	GetSealInfoByOpenTime();
}

void CLevelSealMgr::InitTypeInfoFromCfg(UINT32 uType)
{
	LevelSealTypeTable::RowData* pRow = m_oTypeTable.GetByType(uType);
	if (NULL == pRow)
	{
		LogWarn("Can't find LevelSealType config for type %d", uType);
		return ;
	}
	//热加载配置时重新计算一遍;
	GetSealInfoByOpenTime();
	SendLevelSealInfoToDB();
}
bool CLevelSealMgr::CheckLevelSealInfo()
{
	time_t now = GameTime::GetTime();
	if ((UINT32)now >= m_SealInfo.m_uEndTime)
	{
		if (m_oTypeTable.Table.size() >= (m_SealInfo.m_uType + 1))
		{
			LevelSealTypeTable::RowData* pRow = m_oTypeTable.GetByType(m_SealInfo.m_uType+1);
			if (NULL == pRow)
			{
				return false;
			}
			LogInfo("levelseal type=%d,start_time=%d,unlocked", m_SealInfo.m_uType, m_SealInfo.m_uStartTime);
			SetUnLockTime(m_SealInfo.m_uType, m_SealInfo.m_uEndTime);
			m_SealInfo.m_uType ++;
			m_SealInfo.m_uLevel = pRow->Level;
			m_SealInfo.m_uStartTime = m_SealInfo.m_uEndTime;
			m_SealInfo.m_uEndTime = OperTime(m_SealInfo.m_uStartTime + pRow->Time * 3600);
			m_SealInfo.m_uTotalCollectCount = 0;
			SendLevelSealInfoToDB();
			//notify 所有玩家解除封印
			NoticeMgr::Instance()->LevelSealNotice(ServerLevelSealNotice);
			PushMgr::Instance()->TriggerByLevelSeal();
			ServerCrossDataSyn::SynDataToWorld();
			NotifyLevelSealInfo();
			return true;
		}
	}
	return false;
}
void CLevelSealMgr::SetCollectCount(UINT32 uCount)
{
	m_SealInfo.m_uTotalCollectCount += uCount;
	SendLevelSealInfoToDB();
	LogInfo("gs inc collect cout,type=%d,start_time=%d,count=%d,total_count=%d", m_SealInfo.m_uType, m_SealInfo.m_uStartTime, uCount, m_SealInfo.m_uTotalCollectCount);
}
void CLevelSealMgr::SetStartServerSaveFlag(bool bFlag)
{
	m_bStartServerSave = bFlag;
}
bool CLevelSealMgr::GetStartServerSaveFlag()
{
	return m_bStartServerSave;
}
void CLevelSealMgr::SetType(UINT32 uType)
{
	LevelSealTypeTable::RowData* pRow = m_oTypeTable.GetByType(uType);
	if (NULL == pRow)
	{
		LogWarn("Can't find LevelSealType config for type %d", uType);
		return ;
	}
	LogInfo("set type,pre_type=%d,type=%d" , m_SealInfo.m_uType, uType);
	m_SealInfo.m_uType = uType;
	m_SealInfo.m_uLevel = pRow->Level;
	if (uType == 1)
	{
		m_SealInfo.m_uStartTime = MSConfig::Instance()->GetGameServerOpenTimeStamp();
	}
	else
	{
		m_SealInfo.m_uStartTime = (UINT32)XCommon::GetDayBeginTime(GameTime::GetTime());
	}
	m_SealInfo.m_uEndTime = OperTime(m_SealInfo.m_uStartTime + pRow->Time * 3600);
	m_SealInfo.m_uTotalCollectCount = 0;
	m_vUnlockInfo.clear();
	SendLevelSealInfoToDB();
	NotifyLevelSealInfo(true);//通知gs;
    ServerCrossDataSyn::SynDataToWorld();
}
void CLevelSealMgr::SetTime(UINT32 uTime)
{
	LogInfo("set levelseal time");
	m_SealInfo.m_uEndTime = uTime;
}

UINT32 CLevelSealMgr::OperTime(UINT32 uTime)
{
	return (UINT32)XCommon::GetDayBeginTime(uTime+XCommon::GetOneDayTime()-1);
}
const stServerLevelSealInfo& CLevelSealMgr::GetLevelSealInfo()
{
	//CheckLevelSealInfo(); 为了防止函数重入 去掉Check
	return m_SealInfo;
}
void CLevelSealMgr::SendLevelSealInfoToDB()
{
	CGlobalConfigDBBuild::Instance()->SetConfigValue(eLevelSealType, ToString(m_SealInfo.m_uType));
	CGlobalConfigDBBuild::Instance()->SetConfigValue(eLevelSealStartTime, ToString(m_SealInfo.m_uStartTime));
	KKSG::LevelSealUnlockInfo roInfo;
	for (auto it = m_vUnlockInfo.begin(); it != m_vUnlockInfo.end(); it ++)
	{
		roInfo.add_type(it->first);
		roInfo.add_unlocktime(it->second);
	}
	CGlobalConfigDBBuild::Instance()->SetConfigValue(eLevelSealUnLockTime, roInfo.SerializeAsString());

	CGlobalConfigDBBuild::Instance()->SetConfigValue(eLevelSealCollectCount, ToString(m_SealInfo.m_uTotalCollectCount));

	CGlobalConfigDBBuild::Instance()->UpdateToDb(eLevelSealType);
	CGlobalConfigDBBuild::Instance()->UpdateToDb(eLevelSealStartTime);
	CGlobalConfigDBBuild::Instance()->UpdateToDb(eLevelSealUnLockTime);
	CGlobalConfigDBBuild::Instance()->UpdateToDb(eLevelSealCollectCount);
}
void CLevelSealMgr::SetUnLockTime(UINT32 uType, UINT32 uTime)
{
	//GM命令修改，有可能反复解封，保存最近一次的解封时间;
	auto it = m_vUnlockInfo.find(uType);
	if (it == m_vUnlockInfo.end())
	{
		m_vUnlockInfo.insert(std::make_pair(uType, uTime));
	}
	else
	{
		it->second = uTime;
	}
}

UINT32 CLevelSealMgr::GetLevelSealMaxLevelByType(UINT32 uType)
{
	const LevelSealTypeTable::RowData* pData = GetLevelSealTypeData(uType);
	if (pData == NULL)
	{
		return UINT32(-1);
	}
	return pData->Level;
}

void CLevelSealMgr::GetSealInfoByOpenTime()
{
	stServerLevelSealInfo oData;
	oData.m_uType = GetSealInitType();
	oData.m_uStartTime = MSConfig::Instance()->GetGameServerOpenTimeStamp();

	while(1)
	{
		LevelSealTypeTable::RowData* pRow = m_oTypeTable.GetByType(oData.m_uType);
		if (pRow == NULL)
		{
			break;
		}
		oData.m_uLevel = pRow->Level;
		UINT32 now = (UINT32)GameTime::GetTime();
		oData.m_uEndTime = OperTime(XCommon::GetDayBeginTime(oData.m_uStartTime) + pRow->Time * 3600);
		if (oData.m_uEndTime > now)//lock
		{
			break;
		}
		if (m_oTypeTable.GetByType(oData.m_uType+1) == NULL)
		{
			break;
		}
		SetUnLockTime(oData.m_uType, oData.m_uEndTime);
		oData.m_uType ++;
		oData.m_uStartTime = oData.m_uEndTime;
	}
	m_SealInfo = oData;
	SetStartServerSaveFlag(true);
	NotifyLevelSealInfo();
	LogInfo("||type=%u,starttime=%u,level=%u", m_SealInfo.m_uType, m_SealInfo.m_uStartTime, m_SealInfo.m_uLevel);
}

UINT32 CLevelSealMgr::GetSealInitType()
{
	if (!MSConfig::Instance()->IsBackFlowServer())
	{
		return 1;
	}
	UINT32 level = MSConfig::Instance()->GetBackFlowLevel();
	UINT32 type = 1;
	while(1)
	{
		LevelSealTypeTable::RowData* pRow = m_oTypeTable.GetByType(type);
		if (pRow == NULL)
		{
			break;
		}
		if (pRow->Level > level)
		{
			break;
		}
		type++;
	}
	return type;
}

bool CLevelSealMgr::CanGetTeamCost(UINT32 sealtype, UINT32 worldlevel, UINT32 rolelevel)
{
	TeamCostScale::RowData* data = m_teamcosttable.GetByseallevel(sealtype);
	if (NULL == data)
	{
		return false;
	}
	UINT32 low = worldlevel > data->level ? worldlevel - data->level : 0;
	if (rolelevel >= low && rolelevel <= worldlevel)
	{
		return true;
	}
	else
	{
		return false;
	}
}
