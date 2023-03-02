#include "pch.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "scene/scene.h"
#include "scene/scenecustompk.h"
#include "scene/scenecustompktwo.h"
#include "scene/scenecustomlogger.h"
#include "scene/scenemanager.h"
#include "scene/scenefinishcb.h"
#include "scene/battleresultlistener.h"
#include "custombattle/rpcg2m_custombattleresultms.h"

// generate by ProtoGen at date: 2017/5/9 22:05:05

RPC_CLIENT_IMPLEMETION(RpcG2M_CustomBattleResultMs, CustomBattleResultMsArg, CustomBattleResultMsRes)

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

static void BattleResult(NormalBattleResultListener& listener, Role* role, int point, int rank, bool iswin)
{
	listener.SetInitStatus(role);
	listener.SetSceneRank(RANK_SSS);

	KKSG::CustomBattleResult* result = listener.GetCustomBattleResult();
	result->set_rank(rank);
	result->set_point(point);
	result->set_result(iswin ? KKSG::PkResult_Win : KKSG::PkResult_Lose);
	result->set_fightgroup(role->GetFightGroup());

	role->SetRecvedReward();
	listener.SetStatisticsInfo(role->GetStatistics());
}

void RpcG2M_CustomBattleResultMs::OnReply(const CustomBattleResultMsArg &roArg, const CustomBattleResultMsRes &roRes, const CUserData &roUserData)
{
	Scene* scene = SceneManager::Instance()->FindBySceneID(roArg.sceneuid());
	if (NULL == scene)
	{
		SSWarn<<"scene is NULL, sceneid:"<<roArg.sceneuid()<<" battleuid:"<<roArg.uid()<<END;
		return;
	}
	CustomBattleLogger* logger = NULL;
	SceneCustomPk* pk = scene->GetCustomPkHandler();
	if (NULL != pk)
	{
		logger = pk->GetLogger();
	}
	else
	{
		SceneCustomPkTwo* pktwo = scene->GetCustomPkTwoHandler();
		if (NULL != pktwo)
		{
			logger = pktwo->GetLogger();
		}
	}

	UINT64 winroleid = 0;
	std::vector<Role*> roles;
	NormalBattleResultListener listener(scene, true);
	for (int i = 0; i < roArg.roleida_size(); ++i)
	{
		UINT64 roleid = roArg.roleida(i);
		if (i >= roRes.ranka_size() || i >= roRes.pointa_size() || i >= roRes.allpointa_size())
		{
			SSWarn<<"size is not same, uid:"<<roArg.uid()<<END;
			continue;
		}
		int point = roRes.pointa(i);
		int allpoint = roRes.allpointa(i);
		int rank = roRes.ranka(i);
		bool iswin = IsWin(roleid, roArg);
		if (NULL != logger)
		{
			logger->PushScore(roleid, iswin, point, allpoint);
		}
		Role* role = RoleManager::Instance()->FindByRoleID(roleid);
		if (NULL != role)	
		{
			if (iswin)
			{
				winroleid = role->GetID();
			}
			BattleResult(listener, role, point, rank, iswin);
			roles.push_back(role);
		}
	}
	for (int i = 0; i < roArg.roleidb_size(); ++i)
	{
		UINT64 roleid = roArg.roleidb(i);
		if (i >= roRes.rankb_size() || i >= roRes.pointb_size() || i >= roRes.allpointb_size())
		{
			SSWarn<<"size is not same, uid:"<<roArg.uid()<<END;
			continue;
		}
		int point = roRes.pointb(i);
		int allpoint = roRes.allpointb(i);
		int rank = roRes.rankb(i);
		bool iswin = IsWin(roleid, roArg);
		if (NULL != logger)
		{
			logger->PushScore(roleid, iswin, point, allpoint);
		}
		Role* role = RoleManager::Instance()->FindByRoleID(roleid);
		if (NULL != role)	
		{
			if (iswin)
			{
				winroleid = role->GetID();
			}
			BattleResult(listener, role, point, rank, iswin);
			roles.push_back(role);
		}
	}

	for (auto i = roles.begin(); i != roles.end(); ++i)
	{
		listener.NotifyClient(*i);
	}
	if (!roles.empty())
	{
		listener.SetWatchPvp(winroleid);
		listener.NotifyWatcher(scene);
	}
	if (NULL != logger)
	{
		logger->End();
	}
}

void RpcG2M_CustomBattleResultMs::OnTimeout(const CustomBattleResultMsArg &roArg, const CUserData &roUserData)
{
}
