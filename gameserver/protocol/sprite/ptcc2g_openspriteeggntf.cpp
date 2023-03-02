#include "pch.h"
#include "sprite/ptcc2g_openspriteeggntf.h"
#include "unit/rolemanager.h"
#include "unit/role.h"
#include "gamelogic/lotteryrecord.h"
#include "gamelogic/item.h"
#include "gamelogic/noticemgr.h"

// generate by ProtoGen at date: 2016/9/14 14:37:07

void PtcC2G_OpenSpriteEggNtf::Process(UINT32 dwConnID)
{
	Role* pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if(NULL == pRole)
	{
		SSError << "Cannot Find Role, SessionID:" << m_sessionID << END; 
		return; 
	}
	LotteryRecord* pRecord = pRole->Get<LotteryRecord>();
	if(pRecord->IsHaveAndDelete(m_Data.itemid()))
	{
		XItem item(0, m_Data.itemid(), 1, SPRITE);
		NoticeMgr::Instance()->LotteryNotice(pRole, &item);
	}
}
