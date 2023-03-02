#include "pch.h"
#include "artifact/rpcc2g_artifactdeitystoveop.h"
#include "unit/rolemanager.h"
#include "unit/role.h"
#include "gamelogic/bag.h"
#include "gamelogic/item.h"
#include "gamelogic/artifactmgr.h"

// generate by ProtoGen at date: 2017/7/31 20:43:39

RPC_SERVER_IMPLEMETION(RpcC2G_ArtifactDeityStoveOp, ArtifactDeityStoveOpArg, ArtifactDeityStoveOpRes)

void RpcC2G_ArtifactDeityStoveOp::OnCall(const ArtifactDeityStoveOpArg &roArg, ArtifactDeityStoveOpRes &roRes)
{
	Role* pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (pRole == NULL)
	{
		roRes.set_errorcode(KKSG::ERR_INVALID_REQUEST);
		return;
	}
	bool onBody = false;
	KKSG::ErrorCode code = KKSG::ERR_UNKNOWN;
	switch (roArg.type())
	{
	case KKSG::ArtifactDeityStove_Recast:
		{
			XItem* pItem = pRole->Get<Bag>()->FindUniqueItemInBag(roArg.uid1());
			if (pItem == NULL)
			{
				pItem = pRole->Get<Bag>()->FindUniqueArtifactOnBody(roArg.uid1());
				onBody = true;
			}
			if (pItem == NULL || pItem->type != ARTIFACT)
			{
				roRes.set_errorcode(KKSG::ERR_ITEM_NOTEXIST);
				return;
			}
			code = ArtifactMgr::Instance()->RecastArtifact(pRole, pItem);
			break;
		}
	case KKSG::ArtifactDeityStove_Fuse:
		{
			XItem* pItem1 = pRole->Get<Bag>()->FindUniqueItemInBag(roArg.uid1());
			XItem* pItem2 = pRole->Get<Bag>()->FindUniqueItemInBag(roArg.uid2());
			if (pItem1 == NULL)
			{
				pItem1 = pRole->Get<Bag>()->FindUniqueArtifactOnBody(roArg.uid1());
				onBody = true;
			}
			if (pItem1 == NULL || pItem1->type != ARTIFACT || pItem2 == NULL || pItem2->type != ARTIFACT)
			{
				roRes.set_errorcode(KKSG::ERR_ITEM_NOTEXIST);
				return;
			}
			code = ArtifactMgr::Instance()->FuseArtifact(pRole, pItem1, pItem2, roArg.isusedstone());
			break;
		}
	case KKSG::ArtifactDeityStove_Inscription:
		{
			XItem* pItem1 = pRole->Get<Bag>()->FindUniqueItemInBag(roArg.uid1());
			XItem* pItem2 = pRole->Get<Bag>()->FindUniqueItemInBag(roArg.uid2());
			if (pItem1 == NULL)
			{
				pItem1 = pRole->Get<Bag>()->FindUniqueArtifactOnBody(roArg.uid1());
				onBody = true;
			}
			if (pItem1 == NULL || pItem1->type != ARTIFACT || pItem2 == NULL || pItem2->type != INSCRIPTION)
			{
				roRes.set_errorcode(KKSG::ERR_ITEM_NOTEXIST);
				return;
			}
			code = ArtifactMgr::Instance()->InscriptionArtifact(pRole, pItem1, pItem2);
			break;
		}
	default:
		{
			roRes.set_errorcode(KKSG::ERR_UNKNOWN);
			return;
		}
	}

	roRes.set_errorcode(code);
	if (code == KKSG::ERR_SUCCESS && onBody)
	{
		pRole->RecalcuateAttributeAndNotify(RecalAttr_Artifact);
		pRole->Get<Bag>()->UpdateArtifactEffectData();
	}
}

void RpcC2G_ArtifactDeityStoveOp::OnDelayReplyRpc(const ArtifactDeityStoveOpArg &roArg, ArtifactDeityStoveOpRes &roRes, const CUserData &roUserData)
{
}
