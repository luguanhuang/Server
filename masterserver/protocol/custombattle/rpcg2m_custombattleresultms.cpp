#include "pch.h"
#include "custombattle/custombattlemgr.h"
#include "custombattle/custombattleconfig.h"
#include "custombattle/rpcg2m_custombattleresultms.h"

// generate by ProtoGen at date: 2017/5/9 22:05:05

RPC_SERVER_IMPLEMETION(RpcG2M_CustomBattleResultMs, CustomBattleResultMsArg, CustomBattleResultMsRes)

static bool IsWin(UINT64 roleid, const CustomBattleResultMsArg &roArg)
{
	for (int i = 0; i < roArg.rolewin_size(); ++i)
	{
		if (roArg.rolewin(i) == roleid)
		{
			return true;
		}
	}
	return false;
}

static UINT32 GetPoint(CustomBattle* battle, UINT64 roleid)
{
	UINT32 temppoint = battle->GetPoint(roleid);
	if ((UINT32)(-1) == temppoint)
	{
		SSWarn<<"roleid:"<<roleid<<" point invalid"<<END;
		temppoint = 1500;
	}
	return temppoint;
}

void RpcG2M_CustomBattleResultMs::OnCall(const CustomBattleResultMsArg &roArg, CustomBattleResultMsRes &roRes)
{
	CustomBattle* battle = CustomBattleMgr::Instance()->GetBattle(roArg.uid());
	if (NULL == battle)
	{
		SSWarn<<"invalid battleuid:"<<roArg.uid()<<END;
		return;
	}
	// point
	UINT32 basepointa = 0;
	for (int i = 0; i < roArg.roleida_size(); ++i)
	{
		UINT32 temppoint = GetPoint(battle, roArg.roleida(i));
		basepointa += temppoint;
		roRes.add_ranka(battle->GetRank(roArg.roleida(i)));
	}
	if (0 != roArg.roleida_size())
	{
		basepointa = basepointa/roArg.roleida_size();
	}
	else
	{
		SSWarn<<"uid:"<<roArg.uid()<<" rolea size is 0"<<END;
		return;
	}
	UINT32 basepointb = 0;
	for (int i = 0; i < roArg.roleidb_size(); ++i)
	{
		UINT32 temppoint = GetPoint(battle, roArg.roleidb(i));
		basepointb += temppoint;
		roRes.add_rankb(battle->GetRank(roArg.roleidb(i)));
	}
	if (0 != roArg.roleidb_size())
	{
		basepointb = basepointb/roArg.roleidb_size();
	}
	else
	{
		SSWarn<<"uid:"<<roArg.uid()<<" roleb size is 0"<<END;
		return;
	}

	// calculate
	for (int i = 0; i < roArg.roleida_size(); ++i)
	{
		UINT64 roleid = roArg.roleida(i);
		UINT32 temppoint = battle->GetPoint(roleid);
		if ((UINT32)(-1) == temppoint)
		{
			SSWarn<<"roleid:"<<roleid<<" point invalid"<<END;
			roRes.add_pointa(0);
			roRes.add_allpointa(0);
			roRes.set_ranka(i, 0);
		}
		else
		{
			int point = CCustomBattleConfig::Instance()->GetPkPoint(temppoint, (int)temppoint - (int)basepointb, IsWin(roleid, roArg) ? 1 : -1);
			battle->AddPoint(roleid, point);
			roRes.add_pointa(point);
			roRes.add_allpointa(battle->GetPoint(roleid));

			// rank
			int currentrank = battle->GetRank(roleid);
			int rankspan = roRes.ranka(i) - currentrank;
			roRes.set_ranka(i, rankspan);

			SSInfo<<"battleuid:"<<roArg.uid()<<" rolea:"<<roleid<<" point:"<<point<<" rankspan:"<<rankspan<<END;
		}
	}
	for (int i = 0; i < roArg.roleidb_size(); ++i)
	{
		UINT64 roleid = roArg.roleidb(i);
		UINT32 temppoint = battle->GetPoint(roleid);
		if ((UINT32)(-1) == temppoint)
		{
			SSWarn<<"roleid:"<<roleid<<" point invalid"<<END;
			roRes.add_pointb(0);
			roRes.add_allpointb(0);
			roRes.set_rankb(i, 0);
		}
		else
		{
			int point = CCustomBattleConfig::Instance()->GetPkPoint(temppoint, (int)temppoint - (int)basepointa, IsWin(roleid, roArg) ? 1 : -1);
			battle->AddPoint(roleid, point);
			roRes.add_pointb(point);
			roRes.add_allpointb(battle->GetPoint(roleid));

			// rank
			int currentrank = battle->GetRank(roleid);
			int rankspan = roRes.rankb(i) - currentrank;
			roRes.set_rankb(i, rankspan);

			SSInfo<<"battleuid:"<<roArg.uid()<<" roleb:"<<roleid<<" point:"<<point<<" rankspan:"<<rankspan<<END;
		}
	}
}

void RpcG2M_CustomBattleResultMs::OnDelayReplyRpc(const CustomBattleResultMsArg &roArg, CustomBattleResultMsRes &roRes, const CUserData &roUserData)
{
}
