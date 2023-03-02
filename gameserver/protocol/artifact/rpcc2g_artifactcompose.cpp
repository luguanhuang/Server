#include "pch.h"
#include "artifact/rpcc2g_artifactcompose.h"
#include "unit/rolemanager.h"
#include "unit/role.h"
#include "gamelogic/artifactmgr.h"
#include "gamelogic/item.h"
#include "gamelogic/bagtransition.h"
#include "define/tlogenum.h"


// generate by ProtoGen at date: 2017/5/6 11:06:29

RPC_SERVER_IMPLEMETION(RpcC2G_ArtifactCompose, ArtifactComposeArg, ArtifactComposeRes)

void RpcC2G_ArtifactCompose::OnCall(const ArtifactComposeArg &roArg, ArtifactComposeRes &roRes)
{
	Role* pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (pRole == NULL)
	{
		roRes.set_errorcode(KKSG::ERR_INVALID_REQUEST);
		return;
	}
	
	switch (roArg.type())
	{
	case KKSG::ArtifactCompose_Single:
		{
			std::set<UINT64> uids;
			for (int i = 0; i < roArg.uids_size(); ++i)
			{
				uids.insert(roArg.uids(i));
			}
			XItem giveItem;
			KKSG::ErrorCode code = ArtifactMgr::Instance()->ComposeArtifact(pRole, uids, giveItem);
			roRes.set_errorcode(code);
			if (code == KKSG::ERR_SUCCESS)
			{
				roRes.set_newuid(giveItem.uid);
				BagTakeItemTransition take(pRole);
				take.SetReason(ItemFlow_Artifact, ItemFlow_ArtifactCompose_Take);
				for (auto i = uids.begin(); i != uids.end(); ++i)
				{
					take.TakeUniqueItem(*i, 1);
				}
				take.NotifyClient();
				BagGiveItemTransition give(pRole);
				give.SetReason(ItemFlow_Artifact, ItemFlow_ArtifactCompose_Give);	
				give.GiveItem(&giveItem);
				give.NotifyClient();
			}
			break;
		}
	case KKSG::ArtifactCompose_Multi:
		{
			if (roArg.qualitys_size() == 0)
			{
				roRes.set_errorcode(KKSG::ERR_FAILED);
				return;
			}
			bool isSucceed = false;
			for (int i = 0; i < roArg.qualitys_size(); ++i)
			{
				ArtifactMgr::Instance()->MultiCompose(pRole, roArg.level(), roArg.qualitys(i), roRes);
				isSucceed |= roRes.errorcode() == KKSG::ERR_SUCCESS;
			}
			if (isSucceed)
			{
				roRes.set_errorcode(KKSG::ERR_SUCCESS);
			}
			
			break;
		}
	default:
		{
			roRes.set_errorcode(KKSG::ERR_UNKNOWN);
			break;
		}
	}
	
}

void RpcC2G_ArtifactCompose::OnDelayReplyRpc(const ArtifactComposeArg &roArg, ArtifactComposeRes &roRes, const CUserData &roUserData)
{
}
