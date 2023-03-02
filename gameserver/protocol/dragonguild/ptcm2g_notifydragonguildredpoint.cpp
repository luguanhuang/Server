#include "pch.h"
#include "dragonguild/ptcm2g_notifydragonguildredpoint.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "dragonguild/dragonguildrecord.h"
#include "dragonguild/dragonguildmgr.h"
#include "define/systemiddef.h"
// generate by ProtoGen at date: 2017/9/13 17:33:00

void PtcM2G_NotifyDragonGuildRedPoint::Process(UINT32 dwConnID)
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
		LogWarn("dragonguild record not find session[%llu]", m_sessionID);
		return;
	}
	switch (m_Data.systemid())
	{
	case SYS_DRAGONGUILD_LIVENESS:
		{
			// 检查活跃红点
			CDragonGuildSimple* guild = DragonGuildSimpleMgr::Instance()->GetDragonGuildByRoleId(pRole->GetID());
			if (guild != NULL)
			{
				UINT32 level = guild->GetDragonGuildLvl();
				record->CheckChest(level, m_Data.liveness());
			}
			break;
		}
	case SYS_DRAGONGUILD_TASK:
		{
			// 检查任务红点
			record->CheckTaskChest(m_Data.tasktype(), m_Data.taskid());
			break;
		}
	default:
		LogWarn("invalid redpoint systemid[%u]", m_Data.systemid());
	}
}
