#include "pch.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "define/systemiddef.h"
#include "custombattle/custombattlerole.h"
#include "custombattle/custombattlerolemgr.h"
#include "custombattle/rpcm2k_custombattlecheckvalid.h"

// generate by ProtoGen at date: 2017/5/8 11:55:59

RPC_CLIENT_IMPLEMETION(RpcM2K_CustomBattleCheckValid, CustomBattleCheckValidArg, CustomBattleCheckValidRes)

void RpcM2K_CustomBattleCheckValid::OnReply(const CustomBattleCheckValidArg &roArg, const CustomBattleCheckValidRes &roRes, const CUserData &roUserData)
{
	if (KKSG::ERR_SUCCESS == roRes.errorcode())
	{
		if (roRes.canreward())
		{
			CRole* role = CRoleManager::Instance()->GetByRoleID(roArg.roleid());
			if (NULL != role)
			{
				role->HintNotify(CustomBattle_Custom, false);
			}
		}
		SSInfo<<"check uid:"<<roArg.uid()<<" roleid:"<<roArg.roleid()<<" succeed"<<END;
	}
	else
	{
		SCustomBattleRole* roleinfo = CustomBattleRoleMgr::Instance()->GetInfo(roArg.roleid());
		if (roArg.uid() == roleinfo->GetJoinId())
		{
			roleinfo->ClearBattle();
			SSWarn<<"battle uid:"<<roArg.uid()<<"is invalid, delete from role:"<<roArg.roleid()<<END;
		}
		else
		{
			SSWarn<<"battle uid:"<<roArg.uid()<<"is invalid,  role:"<<roArg.roleid()<<" join uid:"<<roleinfo->GetJoinId()<<END;
		}
	}
}

void RpcM2K_CustomBattleCheckValid::OnTimeout(const CustomBattleCheckValidArg &roArg, const CUserData &roUserData)
{
}
