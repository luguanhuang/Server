#include "pch.h"
#include "qa/ptcm2g_giveanserqarewardntf.h"
#include "unit/rolemanager.h"
#include "unit/role.h"
#include "utility/tlogger.h"
#include "define/tlogenum.h"
#include "gamelogic/bagtransition.h"
#include "define/qadef.h"

// generate by ProtoGen at date: 2016/10/8 22:44:58

void PtcM2G_GiveAnserQARewardNtf::Process(UINT32 dwConnID)
{
	Role* role = RoleManager::Instance()->FindBySession(m_sessionID);
	if (NULL == role)
	{
		return;
	}

	// 记录tlog
	TAnswerFlow oLog(role);
	oLog.m_iAnswerType = m_Data.type();
	oLog.m_iResultID = m_Data.is_correct() ? TX_BATTLE_SUCCESS:TX_BATTLE_FAIL;
	oLog.m_uQuestionID = m_Data.questionid();
	oLog.m_iIndex = m_Data.curindex() + 1;
	oLog.Do();

	// 奖励
	BagGiveItemTransition transition(role);
	transition.SetReason(ItemFlow_QA, ItemFlow_QA_AnswerRight);

	for (INT32 i = 0; i < m_Data.items_size(); ++i)
	{
		const KKSG::ItemBrief& item = m_Data.items(i);
		transition.GiveItem(item.itemid(), item.itemcount());
	}
	transition.NotifyClient();

	if (m_Data.type() == QATYPE_GUILD)
	{
		role->OnQAFinish();
	}

	SSInfo << "Give QA Answer Reward, RoleId: " << role->GetID() << END;
}
