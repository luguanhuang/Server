#include "pch.h"
#include "battlefield/rpcc2m_enterbattlereadyscene.h"
#include "role/rolemanager.h"
#include "table/MultActivityMgr.h"
#include "battlefield/battlefieldmgr.h"
#include "team/teamforceop.h"
// generate by ProtoGen at date: 2017/8/31 12:29:08

RPC_SERVER_IMPLEMETION(RpcC2M_EnterBattleReadyScene, EnterBattleReadySceneArg, EnterBattleReadySceneRes)

void RpcC2M_EnterBattleReadyScene::OnCall(const EnterBattleReadySceneArg &roArg, EnterBattleReadySceneRes &roRes)
{
	CRole* pRole = CRoleManager::Instance()->GetBySession(m_sessionID);
	if(NULL == pRole)
	{
		roRes.set_error(ERR_INVALID_REQUEST);
		return;	
	}

	bool bRet	= MultActivityMgr::Instance()->IsActivityTimeOpen(MULT_ACTIVITY_BATTLEFIELD);
	if (!bRet)
	{
		roRes.set_error(KKSG::ERR_BATTLEFIELD_NOT_OPEN);
		return;	
	}

	bRet = BattleFieldMgr::Instance()->IsLevelOk(pRole);
	if (!bRet)
	{
		roRes.set_error(KKSG::ERR_BATTLEFIELD_LEVEL);
		return;	
	}
	std::vector<CRole *> vecSingle;
	vecSingle.push_back(pRole);
	TeamForceOp oper;
	oper.LeaveTeam(vecSingle);
	BattleFieldMgr::Instance()->EnterReadyScene(pRole);
	roRes.set_error(KKSG::ERR_SUCCESS);

}

void RpcC2M_EnterBattleReadyScene::OnDelayReplyRpc(const EnterBattleReadySceneArg &roArg, EnterBattleReadySceneRes &roRes, const CUserData &roUserData)
{
}
