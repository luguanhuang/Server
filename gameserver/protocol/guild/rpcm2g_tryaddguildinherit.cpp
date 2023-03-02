#include "pch.h"
#include "guild/rpcm2g_tryaddguildinherit.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "guild/guildrecord.h"
#include "guild/guildinherit.h"
#include "scene/scenemanager.h"
#include "scene/scene.h"
#include <time.h>
#include "utility/tlogger.h"

// generate by ProtoGen at date: 2016/11/17 16:07:00

RPC_SERVER_IMPLEMETION(RpcM2G_tryAddGuildInherit, tryAddGuildInheritArg, tryAddGuildInheritRes)

void RpcM2G_tryAddGuildInherit::OnCall(const tryAddGuildInheritArg &roArg, tryAddGuildInheritRes &roRes)
{
	Role* pRoleOne = RoleManager::Instance()->FindByRoleID(roArg.roleone());
	Role* pRoleTwo = RoleManager::Instance()->FindByRoleID(roArg.roletwo());
	if (pRoleOne == NULL || pRoleTwo == NULL)
	{
		roRes.set_errorcode(ERR_ROLE_NOT_ONLINE);
		return;
	}	

	if (NULL == pRoleOne->GetCurrScene() || NULL == pRoleTwo->GetCurrScene())
	{
		roRes.set_errorcode(ERR_GUILD_INHERIT_MAP_WRONG);
		return;
	}

	if(!pRoleOne->StateCanChangeTo(KKSG::OutLook_Inherit) || !pRoleTwo->StateCanChangeTo(KKSG::OutLook_Inherit))
	{
		roRes.set_errorcode(KKSG::ERR_STATE_CANTCHANGE);
		return;
	}

	Role* pTea = NULL;
	Role* pStu = NULL;
	if (pRoleOne->GetLevel() > pRoleTwo->GetLevel())
	{
		pTea = pRoleOne;
		pStu = pRoleTwo;
	}
	else
	{
		pTea = pRoleTwo;
		pStu = pRoleOne;
	}

	/*
	if ((pTea->GetLevel() - pStu->GetLevel()) < GetGlobalConfig().GuildInheritRoleLvlGap)
	{
		roRes.set_errorcode(ERR_GUILD_INHERIT_GAP);
		return ;
	}
	*/

	if (pTea->Get<CGuildRecord>()->GetInheritTeaTime() >= GetGlobalConfig().GuildInheritTeaLimit)
	{
		if (pTea->GetID() == roArg.roleone())
		{
			roRes.set_errorcode(ERR_GUILD_INHERIT_TIMES);
			return ;
		}
		else
		{
			roRes.set_errorcode(ERR_GUILD_INHERIT_OTHER_TIMES);
			return ;
		}
	}

	if (pStu->Get<CGuildRecord>()->GetInheritStuTime() >= GetGlobalConfig().GuildInheritStuLimit)
	{
		if (pStu->GetID() == roArg.roleone())
		{
			roRes.set_errorcode(ERR_GUILD_INHERIT_TIMES);
			return ;
		}
		else
		{
			roRes.set_errorcode(ERR_GUILD_INHERIT_OTHER_TIMES);
			return ;
		}
	}

	/*
	if (pTea->Get<CGuildRecord>()->GetInheritTeaTime() >= GetGlobalConfig().GuildInheritTeaLimit
		|| pStu->Get<CGuildRecord>()->GetInheritStuTime() >= GetGlobalConfig().GuildInheritStuLimit)
	{
		roRes.set_errorcode(ERR_GUILD_INHERIT_TIMES);
		return ;
	}
	*/

	UINT32 cdTime = pTea->Get<CGuildRecord>()->GetGuildInheritCDTime(); 
	UINT32 nowTime = time(NULL);
	if (nowTime > cdTime && (nowTime - cdTime) < (GetGlobalConfig().GuildInheritCDTime * 60))
	{
		roRes.set_errorcode(ERR_GUILD_INHERIT_CD_TIME);
		return ;
	}

	if (roArg.isdo())
	{
		if (CGuildInherit::Instance()->IsInherit(pTea->GetID()) || CGuildInherit::Instance()->IsInherit(pStu->GetID()))
		{
			roRes.set_errorcode(ERR_GUILD_INHERIT_NOT_EXIT);
			return ;
		}
		
		// push
		CGuildInherit::Instance()->Push(pTea->GetID(), pStu->GetID());
		// change scene 
		if (pTea->GetCurrScene()->GetSceneTemplateID() == GUILD_MAP_ID)
		{
			CGuildInherit::Instance()->Add(pTea);
		}
		else
		{
			SceneManager::Instance()->ChangeScene(pTea, GUILD_MAP_ID);
		}

		if (pStu->GetCurrScene()->GetSceneTemplateID() == GUILD_MAP_ID)
		{
			CGuildInherit::Instance()->Add(pStu);
		}
		else
		{
			SceneManager::Instance()->ChangeScene(pStu, GUILD_MAP_ID);
		}

		/*
		pTea->Get<CGuildRecord>()->AddInheritTeaTime();
		pStu->Get<CGuildRecord>()->AddInheritStuTime();
		pTea->Get<CGuildRecord>()->SetGuildInheritTime(nowTime);
		*/

		TGuildInheritFlow oLog(pRoleOne);
		oLog.pInheritRole = pRoleTwo;
		oLog.Do();
	}

	roRes.set_errorcode(ERR_SUCCESS);

	pRoleOne->OnGuildInherit();
	pRoleTwo->OnGuildInherit();
}

void RpcM2G_tryAddGuildInherit::OnDelayReplyRpc(const tryAddGuildInheritArg &roArg, tryAddGuildInheritRes &roRes, const CUserData &roUserData)
{
}
