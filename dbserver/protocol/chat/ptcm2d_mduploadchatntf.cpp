#include "pch.h"
#include "chat/ptcm2d_mduploadchatntf.h"
#include "db/mysqlmgr.h"
#include "db/task/chatwritetask.h"


// generate by ProtoGen at date: 2016/8/1 22:49:42

void PtcM2D_MDUpLoadChatNtf::Process(UINT32 dwConnID)
{
	SSInfo << "Handle Master Upload Chat Info Notify, Channel:" << m_Data.channel() << ", StoreKey:" << m_Data.storekey() << ", connID:" << dwConnID << END;
	CChatWriteTask* poTask = new CChatWriteTask();
	poTask->m_dwChannel = m_Data.channel();
	poTask->m_dwStoreKey = m_Data.storekey();
	poTask->m_strChatList = m_Data.chatinfolist();
	CMysqlMgr::Instance()->PushTask(poTask);
}
