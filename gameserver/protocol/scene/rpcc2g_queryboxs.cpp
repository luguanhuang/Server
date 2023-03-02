#include "pch.h"
#include "scene/rpcc2g_queryboxs.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "scene/scene.h"

// generate by ProtoGen at date: 2017/9/29 11:26:20

RPC_SERVER_IMPLEMETION(RpcC2G_QueryBoxs, QueryBoxsArg, QueryBoxsRes)

void RpcC2G_QueryBoxs::OnCall(const QueryBoxsArg &roArg, QueryBoxsRes &roRes)
{
	Role* role = RoleManager::Instance()->FindBySession(m_sessionID);
	if (NULL == role)
	{
		roRes.set_errorcode(KKSG::ERR_INVALID_REQUEST);
		return;
	}
	Scene* scene = role->GetCurrScene();
	if (NULL == scene)
	{
		roRes.set_errorcode(KKSG::ERR_INVALID_REQUEST);
		return;
	}
	TeamSelectReward* selectreward = scene->GetTeamSelectReward();
	if (NULL == selectreward)
	{
		roRes.set_errorcode(KKSG::ERR_FAILED);
		return;
	}
	// state and time
	int finishstate = scene->GetFinishState();
	if (finishstate < SCENE_FINISH_END)
	{
		if (finishstate == SCENE_FINISH_DRAW_BOX)
		{
			int nowtime = (int)time(NULL);
			int finishtime = scene->GetFinishStateExpireTime();
			roRes.set_timeleft(finishtime > nowtime ? finishtime - nowtime : 0);
			roRes.set_errorcode(KKSG::ERR_QUERYBOX_TIMELEFT);
		}
		else
		{
			SSWarn<<"role:"<<role->GetID()<<" client query box at state:"<<finishstate<<END;
			roRes.set_errorcode(KKSG::ERR_INVALID_STATE);
		}
		return;
	}
	// get all box
	std::map<UINT64, RoleBoxRewardInfo>* allbox = selectreward->GetAllBox();
	if (NULL != allbox)
	{
		for (auto i = allbox->begin(); i != allbox->end(); ++i)
		{
			KKSG::BoxInfos* boxinfos = roRes.add_boxinfos();
			boxinfos->set_roleid(i->first);
			boxinfos->set_index(i->second.index);
			for (auto j = i->second.boxs.begin(); j != i->second.boxs.end(); ++j)
			{
				j->item.ToKKSG(boxinfos->add_items());
				boxinfos->add_type(j->type);
			}
		}
	}
	roRes.set_errorcode(KKSG::ERR_SUCCESS);
}

void RpcC2G_QueryBoxs::OnDelayReplyRpc(const QueryBoxsArg &roArg, QueryBoxsRes &roRes, const CUserData &roUserData)
{
}
