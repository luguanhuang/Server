#include "pch.h"
#include "levelsealMgr.h"
#include "levelseal/ptcg2c_levelsealntf.h"
#include "levelsealRecord.h"
#include "unit/rolemanager.h"
#include "globalconfig.h"
#include "noticemgr.h"
#include "util.h"
#include <time.h>
#include "util/XCommon.h"
#include "gamelogic/rolesummarymgr.h"
#include "levelseal/rpcg2m_inclevelsealbosscount.h"
#include "network/mslink.h"
#include "gamelogic/bagtransition.h"
#include "table/globalconfig.h"
#include "spactivity/spactivitymgr.h"
#include "mail/mailsender.h"
#include "servermgr/servercrossdatamgr.h"

INSTANCE_SINGLETON(CLevelSealMgr)

CLevelSealMgr::CLevelSealMgr()
{
}
CLevelSealMgr::~CLevelSealMgr()
{

}
bool CLevelSealMgr::Init()
{
	if (!m_oTable.LoadFile("table/LevelSeal.txt"))
	{
		SSWarn<<"load file table/LevelSeal.txt failed"<<END;
		return false;
	}

	if (!m_oTypeTable.LoadFile("table/LevelSealType.txt"))
	{
		SSWarn<<"load file table/LevelSealType.txt"<<END;
		return false;
	}

	m_TimeHandler = CTimerMgr::Instance()->SetTimer(this, 0, 3000, -1, __FILE__, __LINE__);
	return true;
}
void CLevelSealMgr::Uninit()
{
	CTimerMgr::Instance()->KillTimer(m_TimeHandler);
}
void CLevelSealMgr::ClearFile()
{
	m_oTypeTable.Clear();
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
	return true;
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
	return true;
}
const stServerLevelSealInfo& CLevelSealMgr::GetLevelSealInfo()
{
	return m_SealInfo;
}
float CLevelSealMgr::GetPlayerExtraBuff(UINT32 uLevel, UINT64 roleid)
{
	UINT32 type = 0;
	if (GSConfig::Instance()->IsCrossGS())
	{
		auto pData = ServerCrossDataMgr::Instance()->GetDataByRoleID(roleid);
		if (pData)
		{
			type = pData->sealtype();
		}
	}
	else
	{
		type = m_SealInfo.m_uType;
	}
	for (auto i = m_oTable.Table.begin(); i != m_oTable.Table.end(); ++ i)
	{
		LevelSeal::RowData* rowdata = *i;
		if (NULL == rowdata)
		{
			continue;
		}
		if (rowdata->Type == type && uLevel >= rowdata->BeginLevel && uLevel <= rowdata->EndLevel)
		{
			return rowdata->ExtraBuff;
		}
	}
	return 0.0;
}

void CLevelSealMgr::SetCollectCount(UINT32 uCount)
{
	m_SealInfo.m_uIncCollectCount = uCount;
}

UINT32 CLevelSealMgr::GetUnLockTimeByType(UINT32 uType)
{
	if (m_oUnlockInfo.type_size() == m_oUnlockInfo.unlocktime_size())
	{
		for (INT32 i = 0; i < m_oUnlockInfo.type_size(); i ++)
		{
			if (m_oUnlockInfo.type(i) == uType)
			{
				return m_oUnlockInfo.unlocktime(i);
			}
		}
	}
	return 0;
}
void CLevelSealMgr::OnTimer(UINT32 dwID, UINT32 dwCount)
{
	if (m_SealInfo.m_uIncCollectCount == 0)
	{
		return ;
	}
	RpcG2M_IncLevelSealBossCount* rpc = RpcG2M_IncLevelSealBossCount::CreateRpc();
	rpc->m_oArg.set_type(m_SealInfo.m_uType);
	rpc->m_oArg.set_collectcount(m_SealInfo.m_uIncCollectCount);
	MSLink::Instance()->SendTo(*rpc);
}

LevelSealTypeTable::RowData* CLevelSealMgr::GetSealTypeTableData(UINT32 type)
{
	return m_oTypeTable.GetByType(type);
}

void CLevelSealMgr::SetSealInfo(KKSG::MsLevelSealInfo m_oData)
{
	m_SealInfo.m_uLevel = m_oData.level();
	m_SealInfo.m_uStartTime = m_oData.starttime();
	m_SealInfo.m_uEndTime = m_oData.endtime();
	m_SealInfo.m_uLastUpdateTime = m_oData.updatetime();
	m_oUnlockInfo.CopyFrom(m_oData.unlockinfo());
	if (m_SealInfo.m_uType == m_oData.type())
	{
		if (m_oData.isgmnotify())
		{
			RoleManager::Instance()->LevelSealBroadCast(m_oData.type());
		}
	}
	else
	{
		m_SealInfo.m_uIncCollectCount = 0;
		m_SealInfo.m_uTotalCollectCount = 0;
		m_SealInfo.m_uType = m_oData.type();
		if (!m_oData.isgmnotify())
		{
			//notify 所有玩家解除封印;
			RoleManager::Instance()->LevelSealBroadCast(m_oData.type());
		}
	}

}
void CLevelSealMgr::NotifyLevelSealInfo(Role* role)
{
	if (GSConfig::Instance()->IsCrossGS())
	{
		return;
	}
	CLevelSealRecord* record = role->Get<CLevelSealRecord>();
	if( NULL == record )
	{
		return; 
	}
	//BackAward(role, m_SealInfo.m_uType);
	record->CheckType(m_SealInfo.m_uType);
	PtcG2C_LevelSealNtf ntf;
	ntf.m_Data.set_type(m_SealInfo.m_uType);
	ntf.m_Data.set_endtime(m_SealInfo.m_uEndTime);
	ntf.m_Data.set_totalcollectcount(m_SealInfo.m_uTotalCollectCount);
	ntf.m_Data.set_selfcollectcount(record->GetSelfCollectCount());
	ntf.m_Data.set_selfawardcountindex(record->GetSelfAwardCountIndex());
	role->CheckLevelSealStatus();
	ntf.m_Data.set_status(record->GetButtonStatus());
	role->Send(ntf);

	LogDebug("levelseal type=%u,endtime=%u,status=%u,roleid=%llu", m_SealInfo.m_uType, m_SealInfo.m_uEndTime, ntf.m_Data.status(), role->GetID());
}
INT32 CLevelSealMgr::ExchangeGift(Role* pRole)
{
	LevelSealTypeTable::RowData* pRow = m_oTypeTable.GetByType(m_SealInfo.m_uType);
	if (NULL == pRow)
	{
		return KKSG::ERR_FAILED;
	}

	BagTakeItemTransition take(pRole);
	take.SetReason(ItemFlow_LevelSeal, ItemFlow_LevelSeal_Exchange);
	if (!take.TakeItem( GetGlobalConfig().LevelSealFragmentID,pRow->ExchangeInfo[0]))
	{
		take.RollBack();
		return KKSG::ERR_LEVELSEAL_PROP_NOT_ENGOUTH;
	}
	take.NotifyClient();

	BagGiveItemTransition stTransition(pRole);
	stTransition.SetReason(ItemFlow_LevelSeal, ItemFlow_LevelSeal_Exchange);
	stTransition.GiveItem(pRow->ExchangeInfo[1], 1);
	stTransition.NotifyClient();
	m_SealInfo.m_uIncCollectCount += pRow->ExchangeInfo[0];
	m_SealInfo.m_uTotalCollectCount += pRow->ExchangeInfo[0];//先修改本gs的数据保证前端看到的是对的，后面由ms覆盖;
	
	pRole->Get<CLevelSealRecord>()->CheckType(m_SealInfo.m_uType);
	pRole->Get<CLevelSealRecord>()->AddSelfCollectCount(pRow->ExchangeInfo[0]);
	return KKSG::ERR_SUCCESS;
}

INT32 CLevelSealMgr::GetSelfGift(Role* pRole, INT32 iIndex)
{
	LevelSealTypeTable::RowData* pRow = m_oTypeTable.GetByType(m_SealInfo.m_uType);
	if (NULL == pRow)
	{
		return KKSG::ERR_FAILED;
	}
	CLevelSealRecord* pRecord = pRole->Get<CLevelSealRecord>();
	if (pRecord->GetType() != m_SealInfo.m_uType)
	{
		return KKSG::ERR_FAILED;
	}
	if (iIndex < 0 || iIndex >= (INT32)pRow->PlayerAward.size())
	{
		return KKSG::ERR_FAILED;
	}
	if (iIndex != (pRecord->GetSelfAwardCountIndex() + 1))
	{
		return KKSG::ERR_FAILED;
	}
	//数量不足;
	if (pRow->PlayerAward[iIndex][0] > pRecord->GetSelfCollectCount())
	{
		return KKSG::ERR_FAILED;
	}
	BagGiveItemTransition stTransition(pRole);
	stTransition.SetReason(ItemFlow_LevelSeal, ItemFlow_LevelSealSelfGit);
	stTransition.GiveItem(DRAGON_COIN, pRow->PlayerAward[iIndex][1]);
	stTransition.NotifyClient();

	pRecord->SetSelfAwardCountIndex(iIndex);
	return KKSG::ERR_SUCCESS;
}
void CLevelSealMgr::BackAward(Role* pRole, INT32 iType)
{
	CLevelSealRecord* pRecord = pRole->Get<CLevelSealRecord>();
	if (pRecord->GetType() <= 0 || iType == pRecord->GetType())
	{
		return;
	}
	LevelSealTypeTable::RowData* pRow = m_oTypeTable.GetByType(pRecord->GetType());
	if (NULL == pRow)
	{
		return;
	}
	for (INT32 i = pRecord->GetSelfAwardCountIndex() + 1; i < INT32(pRow->PlayerAward.size()); ++ i)
	{
		if (pRow->PlayerAward[i][0] > pRecord->GetSelfCollectCount())
		{
			break;
		}
		std::vector<ItemDesc> items;
		ItemDesc pdesc(DRAGON_COIN, pRow->PlayerAward[i][1]);
		items.push_back(pdesc);
		MailSender sender;
		sender.SetContent(Mail_System,  GetGlobalConfig().GetConfig(pRow->PlayerAwardText[0].c_str()),  GetGlobalConfig().GetConfig(pRow->PlayerAwardText[1].c_str()));
		sender.SetItems(items);
		sender.Send(pRole->GetID());
	}
	pRecord->CheckType(iType);
}
UINT32 CLevelSealMgr::GetUpdateTime()
{
	return m_SealInfo.m_uLastUpdateTime;
}
void CLevelSealMgr::GetUnLockInfo(std::map<UINT32, UINT32>& UnlockInfoList)
{
	if (m_oUnlockInfo.type_size() == m_oUnlockInfo.unlocktime_size())
	{
		for (INT32 i = 0; i < m_oUnlockInfo.type_size(); i ++)
		{
			UnlockInfoList.insert(std::make_pair(m_oUnlockInfo.type(i), m_oUnlockInfo.unlocktime(i)));
		}
	}
}

void CLevelSealMgr::NotifyClient(Role* pRole)
{
	CLevelSealRecord* record = pRole->Get<CLevelSealRecord>();
	PtcG2C_LevelSealNtf ntf;
	ntf.m_Data.set_type(m_SealInfo.m_uType);
	ntf.m_Data.set_endtime(m_SealInfo.m_uEndTime);
	ntf.m_Data.set_totalcollectcount(m_SealInfo.m_uTotalCollectCount);
	if (record->GetLastStatus() && !pRole->GetLevelSealStatus())
	{
		record->SetButtonStatus(KKSG::BUTTON_STATUS_UNLOCKED);//解封未点击;
	}
	//判断的原因：防止因为GM指令修改的玩家等级太高了，虽然服务器解封等级了，玩家实际还是比服务器等级高;
	if (pRole->GetLevelSealStatus())
	{
		record->SetLastStatus(true);
	}
	else
	{
		record->SetLastStatus(false);
	}
	pRole->CheckLevelSealStatus();
	ntf.m_Data.set_status(record->GetButtonStatus());
	pRole->Send(ntf);
}

UINT32 CLevelSealMgr::GetThisTimeLevelSealType(time_t thisTime)
{
	time_t serverOpenTime = GSConfig::Instance()->GetGameServerOpenTimeStamp();
	UINT32 type = 1;
	while(1)
	{
		LevelSealTypeTable::RowData* conf = GetSealTypeTableData(type);
		if (conf == NULL)
		{
			return 0;
		}
		serverOpenTime += conf->Time * 3600;
		if (serverOpenTime >= thisTime)
		{
			return type;
		}
		type++;
	}
}