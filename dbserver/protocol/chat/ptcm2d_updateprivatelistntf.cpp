#include "pch.h"
#include "chat/ptcm2d_updateprivatelistntf.h"
#include "db/mysqlmgr.h"
#include "db/task/privatechatlistupdatetask.h"

// generate by ProtoGen at date: 2016/8/26 21:27:22

void PtcM2D_UpdatePrivateListNtf::Process(UINT32 dwConnID)
{
	CPrivateChatListUpdateTask* poTask = new CPrivateChatListUpdateTask();
	poTask->m_dwRoleID = m_Data.roleid();
	poTask->m_privateChatList = m_Data.privatechatlist();
	CMysqlMgr::Instance()->PushTask(poTask);
}
