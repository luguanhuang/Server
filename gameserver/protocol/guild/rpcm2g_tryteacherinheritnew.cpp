#include "pch.h"
#include "guild/rpcm2g_tryteacherinheritnew.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
//#include "guild/guildrecord.h"
#include "guild/guildinherit.h"
#include "scene/scene.h"
#include "scene/scenemanager.h"
#include "table/globalconfig.h"
// generate by ProtoGen at date: 2017/6/14 18:38:10

RPC_SERVER_IMPLEMETION(RpcM2G_TryTeacherInheritNew, TryTeacherInheritNewArg, TryTeacherInheritNewRes)

void RpcM2G_TryTeacherInheritNew::OnCall(const TryTeacherInheritNewArg &roArg, TryTeacherInheritNewRes &roRes)
{
	Role* pRoleOne = RoleManager::Instance()->FindByRoleID(roArg.role_one());
	Role* pRoleTwo = RoleManager::Instance()->FindByRoleID(roArg.role_two());
	if (pRoleOne == NULL || pRoleTwo == NULL)
	{
		roRes.set_result( KKSG::ERR_ROLE_NOT_ONLINE);
		return;
	}		

	// 师徒传功的状态检查
	if (CGuildInherit::Instance()->IsInherit(roArg.role_one()) || 
		CGuildInherit::Instance()->IsInherit(roArg.role_two()) )
	{
		roRes.set_result(KKSG::ERR_MENTOR_INHERIT_ING);
		return ;
	}

	if (NULL == pRoleOne->GetCurrScene())
	{
		roRes.set_result(KKSG::ERR_GUILD_INHERIT_MAP_WRONG);
		return;
	}
	if (NULL == pRoleTwo->GetCurrScene())
	{
		roRes.set_result( KKSG::ERR_GUILD_INHERIT_MAP_WRONG);
		return;
	}

	// 检查所在地图是否符合要求
	if (!CGuildInherit::Instance()->IsMapRight(roArg.role_one(), roArg.role_two()))
	{
		roRes.set_result(KKSG::ERR_GUILD_INHERIT_MAP_WRONG);
		return;
	}
	
	CGuildInherit::Instance()->Push(pRoleOne->GetID(), pRoleTwo->GetID(), CGuildInherit::TEACHER_INHERIT);
	if (pRoleOne->GetCurrScene()->GetSceneTemplateID() == HALL_MAP_ID)
	{
		CGuildInherit::Instance()->Add(pRoleOne, CGuildInherit::TEACHER_INHERIT);
	}
	else
	{
		SceneManager::Instance()->ChangeScene(pRoleOne, HALL_MAP_ID);
	}
	if (NULL == pRoleTwo->GetCurrScene())
	{
		roRes.set_result( KKSG::ERR_ROLE_NOT_ONLINE);
		return;
	}
	if (pRoleTwo->GetCurrScene()->GetSceneTemplateID() == HALL_MAP_ID)
	{
		CGuildInherit::Instance()->Add(pRoleTwo, CGuildInherit::TEACHER_INHERIT);
	}
	else
	{
		SceneManager::Instance()->ChangeScene(pRoleTwo, HALL_MAP_ID);
	}
	
		
	roRes.set_result( KKSG::ERR_SUCCESS);
}

void RpcM2G_TryTeacherInheritNew::OnDelayReplyRpc(const TryTeacherInheritNewArg &roArg, TryTeacherInheritNewRes &roRes, const CUserData &roUserData)
{
}
