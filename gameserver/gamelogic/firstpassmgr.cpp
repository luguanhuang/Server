#include "pch.h"
#include <time.h>
#include "firstpassmgr.h"
#include "ranklist/rpcg2m_updatemsranklist.h"
#include "gamelogic/firstpassconfig.h"
#include "scene/scene.h"
#include "scene/sceneteam.h"
#include "foreach.h"
#include "network/mslink.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "gamelogic/firstpassrecord.h"


INSTANCE_SINGLETON(FirstPassMgr)


FirstPassMgr::FirstPassMgr()
{

}

FirstPassMgr::~FirstPassMgr()
{

}

bool FirstPassMgr::Init()
{
	return true;
}

void FirstPassMgr::Uninit()
{

}

void FirstPassMgr::CheckFirstPass(Scene* scene)
{
	if (!scene || scene->GetSceneState() != SCENE_WIN)
	{
		return;
	}
	SceneTeam* pTeam = scene->GetSceneTeam();
	if (!pTeam)
	{
		return;
	}
	const FirstPassTable::RowData* data = FirstPassConfig::Instance()->GetFirstPassDataByScene(scene->GetSceneTemplateID());
	if (!data)
	{
		return;
	}
	std::vector<UINT64> allRoles;
	if (GSConfig::Instance()->IsCrossGS())
	{
		bool bFromOneServer = true;
		UINT32 serverID = 0;
		foreach (i in pTeam->GetAllMemberIDs())
		{
			auto member = pTeam->GetSynMember(*i);
			if (!member)
			{
				bFromOneServer = false;
				break;
			}

			if (!serverID)
			{
				serverID = member->serverid();
				allRoles.push_back(*i);
			}
			else if(serverID == member->serverid())
			{
				allRoles.push_back(*i);
			}
			else
			{
				bFromOneServer = false;
				break;
			}
		}
		if(!bFromOneServer)
		{
			SSError << " cross gs donot handle first pass mapid = " << scene->GetSceneTemplateID() << " sceneuid:" << scene->GetSceneID() << END;
			return;
		}
	}
	else
	{
		allRoles = scene->GetSceneTeam()->GetAllMemberIDs();
	}
	UINT32 starLevel = 0;
	for (UINT32 i = 0; i < data->SceneID.size(); i++)
	{
		if (data->SceneID[i] == scene->GetSceneTemplateID())
		{
			starLevel = i;
			break;
		}
	}

	std::list<Role*> roles = scene->GetAllRoles();
	if (roles.empty())
	{
		return;
	}
	RpcG2M_UpdateMSRankList* rpc = RpcG2M_UpdateMSRankList::CreateRpc();
	rpc->m_oArg.set_ranktype(KKSG::FirstPassRank);
	rpc->m_oArg.set_firstpassid(data->ID);
	KKSG::RankData* info = rpc->m_oArg.mutable_info();
	foreach (i in allRoles)
	{
		info->add_roleids(*i);
	}	
	info->set_starlevel(starLevel);

	(*roles.begin())->SendToMS(*rpc);
}

void FirstPassMgr::OnUpdateMsRankReply(const KKSG::UpdateMSRankListArg &roArg, const KKSG::UpdateMSRankListRes &roRes)
{
	if (roArg.ranktype() != KKSG::FirstPassRank)
	{
		return;
	}
	if (roRes.error() == KKSG::ERR_SUCCESS && roArg.info().starlevel() == 0 && roRes.rank_size())
	{
		for (int i = 0; i < roArg.info().roleids_size(); ++i)
		{
			Role* role = RoleManager::Instance()->FindByRoleID(roArg.info().roleids(i));
			if (!role)
			{
				LogWarn("role:%llu, leaving, first pass id : %u, starlevel : %u", roArg.info().roleids(i), roArg.firstpassid(), roArg.info().starlevel());
				continue;
			}
			int rank = INVALID_RANK;
			for (int j = 0; j < roRes.rank_size(); ++j)
			{
				if (role->GetServerId() == roRes.rank(j).key())
				{
					rank = roRes.rank(j).value();
					break;
				}
			}
			role->Get<CFirstPassRecord>()->UpdateStarOneFirstPassRank(roArg.firstpassid(), rank);
			LogInfo("role:%llu, %s first pass id:%u rank %u", role->GetID(), role->GetName().c_str(), roArg.firstpassid(), rank);
		}
	}
}

