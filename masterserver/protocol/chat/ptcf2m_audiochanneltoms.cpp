#include "pch.h"
#include "chat/ptcf2m_audiochanneltoms.h"
#include "chat/chatmgr.h"
#include "chat/fmmgr.h"

// generate by ProtoGen at date: 2017/1/3 11:04:55

void PtcF2M_AudioChannelToMs::Process(UINT32 dwConnID)
{
	UINT64 nRoleID = m_Data.source().roleid();
	std::vector<UINT64 > roleSet;
	FMMgr::Instance()->GetRoleList(nRoleID,roleSet);
	CChatMgr::Instance()->SendGroup(roleSet,m_Data);
}
