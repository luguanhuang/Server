#include "pch.h"
#include "skill/rpcc2g_bindskill.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "gamelogic/skilllevel.h"
#include "table/XSkillEffectMgr.h"
#include "gamelogic/rolesummarymgr.h"

// generate by ProtoGen at date: 2015/4/14 16:22:17

RPC_SERVER_IMPLEMETION(RpcC2G_BindSkill, BingSkillArg, BindSkillRes)

void RpcC2G_BindSkill::OnCall(const BingSkillArg &roArg, BindSkillRes &roRes)
{
	Role *pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (pRole == NULL)
	{
		SSWarn<<"role is NULL, sessionid:"<<m_sessionID<<END;
		roRes.set_errorcode(ERR_INVALID_REQUEST);
		return;
	}

	if (roArg.slot() < 0 || roArg.slot() >= KKSG::Attack_Max || roArg.slot() >= GlobalConfig::GetInstance().SkillSlotUnlockLevel.size())
	{
		roRes.set_errorcode(ERR_BIND_SKILL_MISSSLOT);
		return;
	}
	if (GlobalConfig::GetInstance().SkillSlotUnlockLevel[roArg.slot()] > pRole->GetLevel())
	{
		roRes.set_errorcode(ERR_BIND_SKILL_MISSSLOT);
		return;
	}
	
	RoleSkillSet *pSkillMgr = pRole->GetRoleSkillLevelMgr();
	///> 客户端不能直接绑ex技能
	if (XSkillEffectMgr::Instance()->IsExSkill(roArg.skillhash(), 0))
	{
		roRes.set_errorcode(ERR_BIND_SKILL_MISSSLOT);
		return;
	}

	UINT32 preSkillhash = pSkillMgr->GetBindSkill(roArg.slot());
	KKSG::ErrorCode errorCode = (KKSG::ErrorCode)pSkillMgr->BindSkillSlot(roArg.slot(), roArg.skillhash());
	///> 检查能不能绑技能
	roRes.set_errorcode(errorCode);

	if (KKSG::ERR_SUCCESS == errorCode)
	{
		Bag* pBag = pRole->Get<Bag>();
		for (UINT32 i = 0; i < SkillEmblemCount; i++) //技能纹章战力
		{
			const SkEmblem* pSk = pBag->GetWearedSkillEmblemBySlot(i);
			if (pSk == NULL)
			{
				continue;
			}
			if (pSk->IsEqualHash(roArg.skillhash()) || pSk->IsEqualHash(preSkillhash))
			{
				pRole->RecalcuateAttributeAndNotify(RecalAttr_Skill);
				break;
			}
		}
		pRole->Get<RoleSkillSet>()->DoTxLog(); 
	}

	for (int i = 0; i < KKSG::Attack_Max; ++i)
	{
		roRes.add_skillslot(pSkillMgr->GetBindSkill(i));
	}
	RoleSummaryMgr::Instance()->UpdateRoleSummary(pRole, SUMMARY_SKILL);
}

void RpcC2G_BindSkill::OnDelayReplyRpc(const BingSkillArg &roArg, BindSkillRes &roRes, const CUserData &roUserData)
{
}
