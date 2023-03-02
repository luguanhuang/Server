#include "pch.h"
#include "bigmelee/rpcc2m_enterbmreadyscene.h"
#include "mayhem/mayhemmgr.h"
#include "role/rolemanager.h"
#include "network/gslink.h"
#include "scene/ptcm2g_enterscenefromms.h"
#include "table/globalconfig.h"
#include "scene/sceneswitch.h"
#include "mayhem/mayhemmgr.h"
#include "define/systemiddef.h"
#include "team/teamforceop.h"

// generate by ProtoGen at date: 2017/6/9 20:09:23

RPC_SERVER_IMPLEMETION(RpcC2M_EnterBMReadyScene, EnterBMReadySceneArg, EnterBMReadySceneRes)

void RpcC2M_EnterBMReadyScene::OnCall(const EnterBMReadySceneArg &roArg, EnterBMReadySceneRes &roRes)
{
	ErrorCode error = ERR_SUCCESS;
	CRole *pRole = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (!pRole) 
	{
		roRes.set_error(KKSG::ERR_INVALID_REQUEST);
		return;
	}
	if (MayhemMsMgr::Instance()->GetStep()!=1)
	{
		roRes.set_error(KKSG::ERR_ACTIVITY);
		return;
	}


	const KKSG::BMCrossNtfData& refData = MayhemMsMgr::Instance()->GetBMCrossNtfData();
	if (refData.has_sceneid()==false||refData.sceneid()==0)
	{
		LogInfo("enter bigmelle ready scene roleid=%llu sceneid=%u line=%llu", pRole->GetID(),refData.sceneid(),refData.gsline());
		roRes.set_error(KKSG::ERR_ACTIVITY);
		return;
	}

	if(pRole->IsSystemOpened(SYS_BIGMELEESTART) == false)
	{
		roRes.set_error(KKSG::ERR_NOCONDITION);
		return;
	}

	bool bFlag = MayhemMsMgr::Instance()->IsCanJoin();
	if (!bFlag)
	{
		roRes.set_error(KKSG::ERR_LOGIN_MAXNUM);
		return;
	}

	std::vector<CRole *> vecSingle;
	vecSingle.push_back(pRole);
	TeamForceOp oper;
	oper.LeaveTeam(vecSingle);

	PtcM2G_EnterSceneFromMs ntf;
	ntf.m_Data.set_roleid(pRole->GetID());
	ntf.m_Data.set_sceneid(refData.sceneid());
	ntf.m_Data.set_gsline(refData.gsline());
	ntf.m_Data.set_mapid(refData.sceneid());
	ntf.m_Data.set_iscross(true);
	KKSG::SceneSwitchData oData;
	bool ret = CSceneSwitch::EnterScene(pRole, refData.sceneid(), oData, refData.gsline(), true, GetGlobalConfig().BigMeleeReadyScene);
	if (ret==false)
	{
		roRes.set_error(ERR_FAILED);
		return;
	}
	roRes.set_group(refData.group());
	roRes.set_error(ERR_SUCCESS);
}

void RpcC2M_EnterBMReadyScene::OnDelayReplyRpc(const EnterBMReadySceneArg &roArg, EnterBMReadySceneRes &roRes, const CUserData &roUserData)
{
}
