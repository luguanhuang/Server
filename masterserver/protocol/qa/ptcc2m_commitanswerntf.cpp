#include "pch.h"
#include "qa/ptcc2m_commitanswerntf.h"
#include "qa/qamgr.h"
#include "role/rolemanager.h"
#include "role/role.h"
#include "qa/ptcm2g_giveanserqarewardntf.h"
#include "network/gslink.h"

// generate by ProtoGen at date: 2016/10/8 22:42:35

void PtcC2M_CommitAnswerNtf::Process(UINT32 dwConnID)
{
	CRole* role = CRoleManager::Instance()->GetBySession(m_sessionID);	
	if(NULL == role)
	{
		return ;
	}
	QARoom* room = QAMgr::Instance()->GetRoomByRoleId(role->GetID());
	if (!room)
	{
		SSError << "Get Room Failed, RoleID:" << role->GetID() << END;
		return; 
	}

	PtcM2G_GiveAnserQARewardNtf ntf;
	ntf.m_sessionID = m_sessionID;
	KKSG::ErrorCode code = room->Answer(role->GetID(), m_Data, ntf.m_Data);

	// 推送GS发放奖励
	if (code == KKSG::ERR_SUCCESS)
	{
		role->SendMsgToGS(ntf);
	}
}
