#include "pch.h"
#include "pet/rpcc2g_petoperation.h"
#include "unit/rolemanager.h"
#include "unit/role.h"
//#include "unit/petop.h"
#include "unit/pet.h"
#include "unit/roleoutlook.h"


// generate by ProtoGen at date: 2016/4/15 11:20:17
RPC_SERVER_IMPLEMETION(RpcC2G_PetOperation, PetOperationArg, PetOperationRes)

void RpcC2G_PetOperation::OnCall(const PetOperationArg &roArg, PetOperationRes &roRes)
{
	Role* pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (NULL == pRole)
	{
		roRes.set_errorcode(ERR_UNKNOWN);
		return;
	}

	LogDebug("RpcC2G_PetOperation uid= %llu ",roArg.uid());

	PetSys* pPetSys = pRole->Get<PetSys>();
	if (pPetSys == NULL)
	{
		roRes.set_errorcode(ERR_UNKNOWN);
		return;
	}

	bool isMoodChange = false;
	bool isHungryChange = false;
	UINT32 errorcode = ERR_UNKNOWN;
	switch (roArg.type())
	{
	case PetFeed:
		{
			if (roArg.has_food())
			{
				errorcode = pPetSys->OPFeed(roArg.uid(), roArg.food().itemid(), roArg.food().itemcount(), isMoodChange, isHungryChange);
				//errorcode = op.Feed(role, petinfo, roArg.food().itemid(), roArg.food().itemcount());
			}
			break;
		}
	case PetFellow:
		{
			errorcode = pPetSys->OPFollow(roArg.uid());
			//errorcode = op.Fellow(role, petinfo);
			break;
		}
	case PetFight:
		{
			errorcode = pPetSys->OPSetFight(roArg.uid());
			//errorcode = op.Fight(role, petinfo);
			break;
		}
	case PetUpdate:
		{
			//errorcode = op.Update(role, petinfo);
			break;
		}
	case PetTouch:
		{
			errorcode = pPetSys->OPTouch(roArg.uid(), isMoodChange);
			//errorcode = op.Touch(role, petinfo);
			break;
		}
	case PetRelease:
		{
			errorcode = pPetSys->OPReleasePet(roArg.uid());
			break;
		}
	case ExpandSeat:
		{
			errorcode = pPetSys->OPExpandSeat();
			break;
		}
	case KKSG::PetExpTransfer:
		{
			errorcode = pPetSys->OPPetExpTransfer(roArg.uid(),roArg.destpet_id());
		}
		break;
	case KKSG::SetPetPairRide:
		{
			errorcode = pPetSys->OPSetPairRide(roArg.uid(), roArg.setpairride());
		}
		break;
	case KKSG::QueryPetPairRideInvite:
		{
			const std::deque<KKSG::PetInviteInfo>* inviteinfo = pPetSys->GetPairRideInvite();
			if (NULL != inviteinfo)
			{
				for (auto i = inviteinfo->begin(); i != inviteinfo->end(); ++i)
				{
					*roRes.add_invite() = *i;
				}
			}
			errorcode = KKSG::ERR_SUCCESS;
		}
		break;
	case KKSG::IgnorePetPairRideInvite:
		{
			pPetSys->ClearPairRideInvite();
			errorcode = KKSG::ERR_SUCCESS;
		}
		break;
	case KKSG::OffPetPairRide:
		{
			pRole->SetState(KKSG::OutLook_Normal);			

			RoleOutLook outlook(pRole);
			outlook.Fill(SUMMARY_STATE, NULL);
			outlook.BroadCast();

			errorcode = KKSG::ERR_SUCCESS;
		}
		break;
	default:
		break;
	}
	roRes.set_errorcode(ErrorCode(errorcode));
	roRes.set_ismoodup(isMoodChange);
	roRes.set_ishuneryup(isHungryChange);
	if (roArg.type() == PetFellow)
	{
		roRes.set_followpetid(pPetSys->GetFollowId());
	}
}

void RpcC2G_PetOperation::OnDelayReplyRpc(const PetOperationArg &roArg, PetOperationRes &roRes, const CUserData &roUserData)
{
}
