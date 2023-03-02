#include "pch.h"
#include "sprite/rpcc2g_spriteoperation.h"
#include "unit/rolemanager.h"
#include "unit/role.h"
#include "gamelogic/spriterecord.h"
#include "gamelogic/sprite.h"

// generate by ProtoGen at date: 2016/9/2 17:37:24

RPC_SERVER_IMPLEMETION(RpcC2G_SpriteOperation, SpriteOperationArg, SpriteOperationRes)

void RpcC2G_SpriteOperation::OnCall(const SpriteOperationArg &roArg, SpriteOperationRes &roRes)
{
	Role *pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (NULL == pRole)
	{
		roRes.set_errorcode(KKSG::ERR_INVALID_REQUEST);
		return;
	}
	if (!roArg.has_type())
	{
		LogWarn("sprite rpc lack of type");
		roRes.set_errorcode(KKSG::ERR_UNKNOWN);
		return;
	}
	CSpriteRecord* pSpriteRecord = pRole->Get<CSpriteRecord>();
	KKSG::ErrorCode errorCode = ERR_UNKNOWN;
	switch (roArg.type())
	{
	case KKSG::Sprite_Feed:
		{
			if (!roArg.has_uid() || !roArg.has_feeditemid())
			{
				LogWarn("sprite feed rpc's data lack of uid or feeditemid");
				return;
			}
			errorCode = pSpriteRecord->SpriteAddExp(roArg.uid(), roArg.feeditemid());
			Sprite* pSprite = pSpriteRecord->GetSprite(roArg.uid());
			if (pSprite != NULL)
			{
				roRes.set_exp(pSprite->GetExp());
			}
			break;
		}
	case KKSG::Sprite_Evolution:
		{
			if (!roArg.has_uid())
			{
				LogWarn("sprite op [%d] rpc's data lack of uid", KKSG::Sprite_Evolution);
				return;
			}
			errorCode = pSpriteRecord->SpriteEvolutionLevelUp(roArg.uid());
			UINT32 ppt = 0;
			if (KKSG::ERR_SUCCESS == pSpriteRecord->GetEvolutionLevelUpPPT(roArg.uid(), ppt))
			{
				roRes.set_exp(ppt);
			}
			break;
		}
	case KKSG::Sprite_Awake:
		{
			if (!roArg.has_uid())
			{
				LogWarn("sprite op [%d] rpc's data lack of uid", KKSG::Sprite_Awake);
				return;
			}
			Sprite* awakeBefore = pSpriteRecord->GetSprite(roArg.uid());
			if (awakeBefore != NULL)
			{
				awakeBefore->SpriteToKKSG(roRes.mutable_awakespritebefore());
			}
			errorCode = pSpriteRecord->SpriteAwake(roArg.uid());
			Sprite* awake = pSpriteRecord->GetNewAwake();
			if (awake != NULL)
			{
				awake->SpriteToKKSG(roRes.mutable_awakesprite());
			}
			break;
		}
	/*case KKSG::Sprite_Awake_Retain:
		{
			pSpriteRecord->SpriteAwakeRetain();
			errorCode = KKSG::ERR_SUCCESS;
			break;
		}
	case KKSG::Sprite_Awake_Replace:
		{
			errorCode = pSpriteRecord->SpriteAwakeReplace();
			break;
		}*/
	case KKSG::Sprite_InFight:
		{
			if (!roArg.has_uid())
			{
				LogWarn("sprite op [%d] rpc's data lack of uid", KKSG::Sprite_InFight);
				return;
			}
			errorCode = pSpriteRecord->SpriteInFight(roArg.uid());
			std::vector<UINT64> vecFight;
			pSpriteRecord->GetInFightList(vecFight);
			for (int i = 0; i < vecFight.size(); i++)
			{
				roRes.add_infight(vecFight[i]);
			}
			break;
		}
	case KKSG::Sprite_OutFight:
		{
			if (!roArg.has_uid())
			{
				LogWarn("sprite op [%d] rpc's data lack of uid", KKSG::Sprite_OutFight);
				return;
			}
			errorCode = pSpriteRecord->SpriteOutFight(roArg.uid());
			std::vector<UINT64> vecFight;
			pSpriteRecord->GetInFightList(vecFight);
			for (int i = 0; i < vecFight.size(); i++)
			{
				roRes.add_infight(vecFight[i]);
			}
			break;
		}
	case KKSG::Sprite_Decompose:
		{
			std::vector<UINT64> sprites;
			for (int i = 0; i < roArg.uids_size(); i++)
			{
				sprites.push_back(roArg.uids(i));
			}
			errorCode = pSpriteRecord->SpritesDecompose(sprites);
			break;
		}
	case KKSG::Sprite_SwapLeader:
		{
			if (!roArg.has_uid())
			{
				LogWarn("sprite op [%d] rpc's data lack of uid", KKSG::Sprite_SwapLeader);
				return;
			}
			errorCode = pSpriteRecord->SpriteSwapLeader(roArg.uid());
			std::vector<UINT64> vecFight;
			pSpriteRecord->GetInFightList(vecFight);
			for (int i = 0; i < vecFight.size(); i++)
			{
				roRes.add_infight(vecFight[i]);
			}
			break;
		}
	case KKSG::Sprite_QueryEvolutionPPT:
		{
			if (!roArg.has_uid())
			{
				LogWarn("sprite op [%d] rpc's data lack of uid", KKSG::Sprite_QueryEvolutionPPT);
				return;
			}
			UINT32 ppt = 0;
			errorCode = pSpriteRecord->GetEvolutionLevelUpPPT(roArg.uid(), ppt);
			if (KKSG::ERR_SUCCESS == errorCode)
			{
				roRes.set_exp(ppt);
			}
			break;
		}
	case KKSG::Sprite_Train:
		{
			if (!roArg.has_uid())
			{
				LogWarn("sprite op [%d] rpc's data lack of uid", KKSG::Sprite_Train);
				return;
			}
			std::vector<UINT32> notToChoose;
			for (int i = 0; i < roArg.nottochoose_size(); i++)
			{
				notToChoose.push_back(roArg.nottochoose(i));
			}
			std::vector<std::pair<UINT32, UINT32>> addAttr;
			errorCode = pSpriteRecord->SpriteTrain(roArg.uid(), notToChoose, addAttr);
			for (auto i = addAttr.begin(); i != addAttr.end(); i++)
			{
				roRes.add_lasttrainattrid((*i).first);
				roRes.add_lasttrainattrvalue((*i).second);
			}
			break;
		}
	case KKSG::Sprite_ResetTrain:
		{
			if (!roArg.has_uid())
			{
				LogWarn("sprite op [%d] rpc's data lack of uid", KKSG::Sprite_ResetTrain);
				return;
			}
			errorCode = pSpriteRecord->SpriteResetTrain(roArg.uid(), roArg.resettrainchoose());
			break;
		}
	case KKSG::Sprite_Rebirth:
		{
			if (!roArg.has_uid())
			{
				LogWarn("sprite op [%d] rpc's data lack of uid", KKSG::Sprite_Rebirth);
				return;
			}
			errorCode = pSpriteRecord->SpriteRebirth(roArg.uid());
			break;
		}
	default:
		{
			break;
		}
	}

	roRes.set_errorcode(errorCode);
}

void RpcC2G_SpriteOperation::OnDelayReplyRpc(const SpriteOperationArg &roArg, SpriteOperationRes &roRes, const CUserData &roUserData)
{
}
