#include "pch.h"
#include "dragonguild/rpcm2g_getdragonguildrecieveinfo.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "dragonguild/dragonguildrecord.h"
// generate by ProtoGen at date: 2017/9/12 21:36:52

RPC_SERVER_IMPLEMETION(RpcM2G_GetDragonGuildRecieveInfo, GetDragonGuildRecieveInfoArg, GetDragonGuildRecieveInfoRes)

void RpcM2G_GetDragonGuildRecieveInfo::OnCall(const GetDragonGuildRecieveInfoArg &roArg, GetDragonGuildRecieveInfoRes &roRes)
{
	Role* pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (pRole == NULL)
	{
		LogWarn("no role by session[%llu]", m_sessionID);
		return;
	}
	DragonGuildRecord* record = pRole->Get<DragonGuildRecord>();
	if(record == NULL)
	{
		LogWarn("no dragonguildrecord by session[%llu]", m_sessionID);
		return;
	}
	for(size_t i = 0; i<roArg.tasklist_size(); ++i)
	{
		KKSG::ErrorCode code = record->GetTaskStatus(KKSG::TASK_NORMAL, roArg.tasklist(i));
		if(code == KKSG::ERR_SUCCESS)
			roRes.add_task_receieved(false);
		else 
			roRes.add_task_receieved(true);
	}

	for(size_t i = 0; i<roArg.achievelist_size(); ++i)
	{
		KKSG::ErrorCode code = record->GetTaskStatus(KKSG::TASK_ACHIVEMENT, roArg.achievelist(i));
		if(code == KKSG::ERR_SUCCESS)
			roRes.add_achieve_recieved(false);
		else 
			roRes.add_achieve_recieved(true);
	}
	
}

void RpcM2G_GetDragonGuildRecieveInfo::OnDelayReplyRpc(const GetDragonGuildRecieveInfoArg &roArg, GetDragonGuildRecieveInfoRes &roRes, const CUserData &roUserData)
{

}
