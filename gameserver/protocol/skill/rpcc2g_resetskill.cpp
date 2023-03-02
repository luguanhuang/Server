#include "pch.h"
#include "skill/rpcc2g_resetskill.h"
#include "unit/rolemanager.h"
#include "unit/role.h"
#include "gamelogic/skillchangedrecord.h"
#include "gamelogic/bagtransition.h"
#include "table/globalconfig.h"
#include "table/ProfessionMgr.h"
#include "gamelogic/takecostmgr.h"
#include "foreach.h"
#include "gamelogic/rolesummarymgr.h"
#include "utility/tlogger.h"

// generate by ProtoGen at date: 2015/4/13 14:16:50

RPC_SERVER_IMPLEMETION(RpcC2G_ResetSkill, ResetSkillArg, ResetSkillRes)


#define RESET_FREE_LEVEL 30

void RpcC2G_ResetSkill::OnCall(const ResetSkillArg &roArg, ResetSkillRes &roRes)
{
	Role *pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (pRole != NULL)
	{
		// common skill
		if (roArg.resettype() == KKSG::RESET_SKILL)
		{
			if (pRole->GetLevel() >= RESET_FREE_LEVEL)
			{
				if (pRole->Get<RoleSkillSet>()->IsFreeResetSkill())
				{
					pRole->Get<RoleSkillSet>()->SetFreeResetSkill(false);
				}
				else
				{
					if (!TakeCostManager::Instance()->TakeCost("SkillResetCost", pRole, 0, ItemFlow_Skill, ItemFlow_Skill_Reset))
					{
						roRes.set_errorcode(KKSG::ERR_ITEM_NOT_ENOUGH);
						roRes.set_prof((KKSG::RoleType)pRole->GetProfession());
						return;
					}
				}
			}

			SkillChangedRecord Record(pRole);
			roRes.set_errorcode(pRole->GetRoleSkillLevelMgr()->ResetSkill());
			roRes.set_prof((KKSG::RoleType)pRole->GetProfession());
			Record.SendNtf();
			
			pRole->RecalcuateAttributeAndNotify(RecalAttr_Skill);
			RoleSummaryMgr::Instance()->UpdateRoleSummary(pRole, SUMMARY_SKILL);

		}
		// reset profession
		else if (roArg.resettype() == KKSG::RESET_PROFESSION)
		{
			if (ProfessionMgr::IsBasicProfession(pRole->GetProfession()))
			{
				roRes.set_errorcode(KKSG::ERR_FAILED);
				roRes.set_prof((KKSG::RoleType)pRole->GetProfession());
				return;
			}

			if (!TakeCostManager::Instance()->TakeCost("SkillResetProfession", pRole, 0, ItemFlow_Skill, ItemFlow_ResetProfession))
			{
				roRes.set_errorcode(KKSG::ERR_ITEM_NOT_ENOUGH);
				roRes.set_prof((KKSG::RoleType)pRole->GetProfession());
				return;
			}

			INT32 oldPro = pRole->GetProfession();
			if (pRole->ResetProfession())
			{
				SkillChangedRecord Record(pRole);
				KKSG::ErrorCode error = pRole->GetRoleSkillLevelMgr()->ResetSkill();
				if (KKSG::ERR_SUCCESS == error)
				{
					Record.SetChanged();
					pRole->GetRoleSkillLevelMgr()->ResetSkillTwo();
				}
				roRes.set_errorcode(error);
				roRes.set_prof((KKSG::RoleType)pRole->GetProfession());
				Record.SendNtf();

				pRole->RecalcuateAttributeAndNotify(RecalAttr_Skill);
				RoleSummaryMgr::Instance()->UpdateRoleSummary(pRole, SUMMARY_SKILL);
				TProfessionFlow oLog(pRole);
				oLog.m_PreProfession = oldPro;
				oLog.m_Op = TX_Reset_Profession;
				oLog.Do();
			}
			else
			{
				roRes.set_errorcode(KKSG::ERR_FAILED);
				roRes.set_prof((KKSG::RoleType)pRole->GetProfession());
			}
		}
		else
		{
			roRes.set_errorcode(KKSG::ERR_FAILED);
			roRes.set_prof((KKSG::RoleType)pRole->GetProfession());
		}
	}
	else
	{
		roRes.set_errorcode(KKSG::ERR_FAILED);
		roRes.set_prof(KKSG::Role_INVALID);
	}
}

void RpcC2G_ResetSkill::OnDelayReplyRpc(const ResetSkillArg &roArg, ResetSkillRes &roRes, const CUserData &roUserData)
{
}
