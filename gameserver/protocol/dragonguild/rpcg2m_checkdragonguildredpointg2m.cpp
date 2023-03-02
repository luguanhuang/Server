#include "pch.h"
#include "dragonguild/rpcg2m_checkdragonguildredpointg2m.h"
#include "unit/rolemanager.h"
#include "dragonguild/dragonguildrecord.h"
#include "dragonguild/dragonguildmgr.h"
// generate by ProtoGen at date: 2017/9/14 17:41:46

RPC_CLIENT_IMPLEMETION(RpcG2M_CheckDragonGuildRedPointG2M, CheckDragonGuildRedPointG2MArg, CheckDragonGuildRedPointG2MRes)

void RpcG2M_CheckDragonGuildRedPointG2M::OnReply(const CheckDragonGuildRedPointG2MArg &roArg, const CheckDragonGuildRedPointG2MRes &roRes, const CUserData &roUserData)
{
	Role* pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (NULL == pRole)
	{
		SSWarn << "role is NULL and session is :" << m_sessionID <<END;
		return;
	}
	DragonGuildRecord* record = pRole->Get<DragonGuildRecord>();
	if(record == NULL)
	{
		SSWarn << "role dragonGuildRecord is NULL and session is :" << m_sessionID <<END;
		return;
	}
	
	//检查活跃红点
	CDragonGuildSimple* guild = DragonGuildSimpleMgr::Instance()->GetDragonGuildByRoleId(pRole->GetID());
	if (!guild)
	{
		return ;
	}
	UINT32 level = guild->GetDragonGuildLvl();
	record->CheckChest(level, roRes.liveness());

	//检查任务红点
	if( (roRes.tasklist_size() == 0) && (roRes.achivelist_size() == 0) )
	{
		return;
	}
	for(size_t i = 0; i<roRes.tasklist_size(); ++i)
	{
		if(record->CheckTaskChest(KKSG::TASK_NORMAL, roRes.tasklist(i)))
			return;
	}
	for(size_t i = 0; i<roRes.achivelist_size(); ++i)
	{
		if(record->CheckTaskChest(KKSG::TASK_ACHIVEMENT, roRes.achivelist(i)))
			return;
	}
}

void RpcG2M_CheckDragonGuildRedPointG2M::OnTimeout(const CheckDragonGuildRedPointG2MArg &roArg, const CUserData &roUserData)
{

}
