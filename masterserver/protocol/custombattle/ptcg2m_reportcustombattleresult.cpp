#include "pch.h"
#include "custombattle/ptcg2m_reportcustombattleresult.h"
#include "custombattle/custombattlerolemgr.h"
#include "custombattle/custombattleconfig.h"
#include "custombattle/custombattlemgr.h"
#include "role/rolesummarymgr.h"

// generate by ProtoGen at date: 2017/5/6 16:57:12

void PtcG2M_ReportCustomBattleResult::Process(UINT32 dwConnID)
{
	UINT64 roleid = m_Data.roleid();
	if (NULL == CRoleSummaryMgr::Instance()->GetRoleSummary(m_Data.roleid()))
	{
		SSWarn<<"invalid roleid:"<<roleid<<END;
		return;
	}
	const KKSG::CustomBattleMatchBattleInfo& battle = m_Data.battle();
	SCustomBattleRole* roleinfo = CustomBattleRoleMgr::Instance()->GetInfo(roleid);
	if (battle.issystem())
	{
		if (!CCustomBattleConfig::Instance()->IsSystem(battle.battleuid()))
		{
			SSWarn<<"invalid battleid:"<<battle.battleuid()<<" roleid:"<<roleid<<END;
			return;
		}
		roleinfo->UpdateWinOrLose(battle.battleuid(), m_Data.iswin());
	}
	else
	{
		///> 自定义更新积分
		roleinfo->UpdatePoint(m_Data.battle().battleuid(), m_Data.point());
	}
	SSInfo<<"roleid:"<<roleid<<" battleid:"<<battle.battleuid()<<" is system:"<<battle.issystem()<<" iswin:"<<m_Data.iswin()<<" point:"<<m_Data.point()<<END;
}
