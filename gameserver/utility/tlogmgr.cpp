#include "pch.h"
#include "tlogmgr.h"
#include "timermgr.h"
#include "tlogger.h"
#include "config.h"
#include <time.h>
#include "tloggerbattle.h"

//#define TOLOG_TEST

#ifdef TOLOG_TEST
#include "unit/role.h"
#include "pb/project.pb.h"
#include "gamelogic/robotgenerator.h"

#define DO_TLOG(logType, pRole) { logType oLog(pRole); oLog.Do(); }
#endif
#include "gamelogic/bag.h"
#include "define/tlogenum.h"
#include "gamelogic/itemconfig.h"
#include "gamelogic/jademgr.h"
#include "gamelogic/spriterecord.h"
#include "gamelogic/sprite.h"
#include "gamelogic/atlas.h"


INSTANCE_SINGLETON(TTag);
TTag::TTag()
{
	m_iUniqueSeq = 0;
}
TTag::~TTag()
{

}
bool TTag::Init()
{
	return true;
}

void TTag::Uninit()
{

}

std::string TTag::GetTagString()
{
	char pBuf[32] = {0};
	sprintf(pBuf, "TPID%04dGSN%02dTM%08XSQ%04X", GSConfig::Instance()->GetServerID()%10000,GSConfig::Instance()->GetLine(), (INT32)time(NULL), (++m_iUniqueSeq &0xffff));
	return pBuf;
}

#define STATE_LOG_INTERVAL	(5 * 60 * 1000)

INSTANCE_SINGLETON(TLogMgr);


TLogMgr::TLogMgr()
:m_hLogTimer(INVALID_HTIMER)
{
}

TLogMgr::~TLogMgr()
{
}

bool TLogMgr::Init()
{
	m_hLogTimer = CTimerMgr::Instance()->SetTimer(&m_oLogTimer, 0, STATE_LOG_INTERVAL, -1, __FILE__, __LINE__);
	if (INVALID_HTIMER == m_hLogTimer)
	{
		SSWarn<<"m_hLogTimer is NULL"<<END;
		return false;
	}

	TSecRoundStartFlow::Init();
	TSecRoundEndFlow::Init();
	TSecRoundEndCount::Init();

	return true;
}

void TLogMgr::Uninit()
{
	if(m_hLogTimer != INVALID_HTIMER)
	{
		CTimerMgr::Instance()->KillTimer(m_hLogTimer);
		m_hLogTimer = INVALID_HTIMER;
	}
}

void TLogMgr::Test()
{
#ifdef TOLOG_TEST
	Role* poRole = RobotGenerator::Instance()->CreateTestRole();

	DO_TLOG(TLoginLog, poRole);
	DO_TLOG(TLogoutLog, poRole);
	DO_TLOG(TMoneyFlowLog, poRole);
	DO_TLOG(TFatigueLog, poRole);
	DO_TLOG(TItemFlowLog, poRole);
	DO_TLOG(TPlayerExpFlowLog, poRole);
	DO_TLOG(TSnsFlowLog, poRole);
	DO_TLOG(TRoundFlowLog, poRole);
	DO_TLOG(TSweepLog, poRole);
	DO_TLOG(TGuideFlowLog, poRole);
	DO_TLOG(TVipLevelFlowLog, poRole);
	DO_TLOG(TActivityFlowLog, poRole);
	DO_TLOG(TTaskFlowLog, poRole);
	DO_TLOG(TLotteryFlowLog, poRole);
	DO_TLOG(TItemEnHanceLog, poRole);
	DO_TLOG(TOpenSystemLog, poRole);
	DO_TLOG(TUIGuideFlowLog, poRole);
	DO_TLOG(TOdViewFlow, poRole);
	DO_TLOG(TGuildFightFlow, poRole);
	DO_TLOG(TBossRushRefreshFlow, poRole);
	DO_TLOG(TAuctionFlow, poRole);
	DO_TLOG(TPayFlow, poRole);
	DO_TLOG(TShopFlow, poRole);
	DO_TLOG(TSendFlowerFlow, poRole);
	DO_TLOG(TAnswerFlow, poRole);
	DO_TLOG(TPkLog, poRole);
	DO_TLOG(TGuildCardFlow, poRole);
	DO_TLOG(TSpriteFlow, poRole);
	DO_TLOG(TPetFlow, poRole);
	DO_TLOG(TSuperRiskFlow, poRole);
	DO_TLOG(TDesignationFlow, poRole);
	DO_TLOG(TSkyCityFlow, poRole);
	DO_TLOG(TSecTalkFlow, poRole);
	DO_TLOG(TDanceFlow, poRole);
	DO_TLOG(TGuildInheritFlow, poRole);
	DO_TLOG(TGuildArenaFlow, poRole);
	DO_TLOG(TSceneFlow, poRole);
	DO_TLOG(TRoundUnlockLog, poRole);
	DO_TLOG(TProfessionFlow, poRole);

	poRole->Uninit();
	delete poRole;
#endif
}

void TLogMgr::TStateLogTimer::OnTimer(UINT32 dwID, UINT32 dwCount)
{
	TServerStateLog oLog;
	oLog.Do();

	TLogMgr::Instance()->Test();
}

void TLogMgr::Logout(Role* pRole)
{
	if (NULL == pRole)
	{
		return;
	}
	TLogoutLog oLog(pRole);
	oLog.Do();

	std::string strtrans = TTag::Instance()->GetTagString();
	std::vector<XItem*> items;
	std::vector<ItemDesc> vlist;
	pRole->Get<Bag>()->GetBodyEmblem(items);
	for (UINT32 i = 0; i < items.size(); i ++)
	{
		if (NULL == items[i])
		{
			return;
		}
		XItem::PrintEmblemTLog(pRole, items[i], Tx_Emblem_Logout, -1, vlist, strtrans);
	}

	items.clear();
	pRole->Get<Bag>()->GetBodyEquip(items);
	for (UINT32 j = 0; j < items.size(); j ++)
	{
		if(NULL == items[j])
		{
			return;
		}
		TLogoutBodyItem olog(pRole);
		olog.SetTransTag(strtrans);
		olog.m_ItemID = items[j]->itemID;
		olog.m_ItemUid = items[j]->uid;
		olog.m_ItemLv = items[j]->GetEnhanceLevel();
		olog.m_QualityID = ItemConfig::Instance()->GetItemQuailty(items[j]->itemID);
		JadeInfo* pJade = items[j]->jadeInfo;
		if (NULL != pJade)
		{
			for (UINT32 i = 0; i < pJade->vecJadeSingle.size() && i < TXLOG_JADENUM; i ++)
			{
				olog.m_vJadeList.push_back(pJade->vecJadeSingle[i].jadeid);
				olog.m_JadeSlotNum++;
			}
			UINT32 slotInfo = CJadeMgr::Instance()->GetSlotInfo(pRole->GetLevel(), XItem::GetPos(items[j]->itemID));
			for (UINT32 i = 0; i < JADESLOTCOUNT; i ++)
			{
				UINT32 slotType = JADESLOTMASK & (slotInfo >> JADESLOTMASKCOUNT * i);
				if (JADESLOTNEVER == slotType || JADESLOTCLOSE == slotType) //没有孔 or 孔关闭
				{
					continue;
				}
				olog.m_JadeTotalSlotNum ++;
			}
		}
		UINT32 count = olog.m_vJadeList.size();
		for(UINT32 i = count; i < TXLOG_JADENUM; i ++)
		{
			olog.m_vJadeList.push_back(0);
		}
		olog.m_Attr.SetItem(items[j]);
		FuseAttr* pFuse = items[j]->fuse;
		if (pFuse)
		{
			olog.m_FuseExp = pFuse->fuseExpCount;
			olog.m_FuseLevel = pFuse->fuseLevel;
		}
		olog.Do();
	}

	items.clear();
	pRole->Get<Bag>()->GetBodyArtifact(items);
	for (UINT32 j = 0; j < items.size(); j ++)
	{
		if(NULL == items[j])
		{
			return;
		}
		XItem::PrintArtifactTLog(pRole, items[j], Tx_Artifact_BodyLogout, strtrans);
	}

	//上阵精灵;
	std::vector<UINT64> vecFight;
	pRole->Get<CSpriteRecord>()->GetInFightList(vecFight);
	for (UINT32 i = 0; i < vecFight.size(); i ++)
	{
		Sprite* pInfo = pRole->Get<CSpriteRecord>()->GetSprite(vecFight[i]);
		if (pInfo)
		{
			pInfo->PrintTLogOut(strtrans);
		}
	}

	pRole->Get<CAtlasSys>()->Logout(strtrans);
}