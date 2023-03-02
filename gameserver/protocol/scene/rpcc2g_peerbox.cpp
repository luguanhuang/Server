#include "pch.h"
#include "scene/rpcc2g_peerbox.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "scene/scene.h"
#include "gamelogic/bagtransition.h"
#include "table/globalconfig.h"

// generate by ProtoGen at date: 2017/9/29 11:25:57

RPC_SERVER_IMPLEMETION(RpcC2G_PeerBox, PeerBoxArg, PeerBoxRes)

void RpcC2G_PeerBox::OnCall(const PeerBoxArg &roArg, PeerBoxRes &roRes)
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

	// valid
	if (!TeamSelectReward::ValidIndex(roArg.index()))
	{
		SSWarn<<"role:"<<role->GetID()<<" invalid index:"<<roArg.index()<<END;
		roRes.set_errorcode(KKSG::ERR_FAILED);
		return;
	}
	RoleBoxRewardInfo* rolebox = selectreward->GetRoleBoxInfo(role->GetID());
	if (NULL == rolebox)
	{
		SSWarn<<"role:"<<role->GetID()<<" role box is NULL"<<END;
		roRes.set_errorcode(KKSG::ERR_FAILED);
		return;
	}
	if (!rolebox->CanPeer())
	{
		SSWarn<<"role:"<<role->GetID()<<" can not peer"<<END;
		roRes.set_errorcode(KKSG::ERR_FAILED);
		return;
	}

	const Sequence<UINT32, 2>* peeritem = scene->GetPeerItem();
	if (NULL == peeritem || peeritem->seq[0] == 0 || peeritem->seq[1] == 0)
	{
		SSWarn<<"sceneid:"<<scene->GetSceneTemplateID()<<" not support peer"<<END;
		roRes.set_errorcode(KKSG::ERR_FAILED);
		return;
	}
	//// take item
	BagTakeItemTransition take(role);
	// set reason
	take.SetReason(ItemFlow_Stage, ItemFlow_Stage_PeerTake);
	if (!take.TakeItem(peeritem->seq[0], peeritem->seq[1]))
	{
		take.RollBack();
		roRes.set_errorcode(KKSG::ERR_ITEM_NOT_ENOUGH);
		return;
	}
	take.NotifyClient();

	// peer
	if (!rolebox->Peer(roArg.index()))
	{
		roRes.set_errorcode(KKSG::ERR_FAILED);
		return;
	}
	BoxRewardInfo* drop = rolebox->GetItem(roArg.index());
	if (NULL != drop)
	{
		drop->item.ToKKSG(roRes.mutable_item());
		roRes.set_type(drop->type);
	}
	roRes.set_errorcode(KKSG::ERR_SUCCESS);
}

void RpcC2G_PeerBox::OnDelayReplyRpc(const PeerBoxArg &roArg, PeerBoxRes &roRes, const CUserData &roUserData)
{
}
