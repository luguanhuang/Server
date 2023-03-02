#include "pch.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "unit/pet.h"
#include "scene/scene.h"
#include "pet/rpcc2g_petoperationother.h"
#include "pet/ptcg2c_petinvitentf.h"
#include "unit/roleoutlook.h"

// generate by ProtoGen at date: 2017/6/16 14:16:19

RPC_SERVER_IMPLEMETION(RpcC2G_PetOperationOther, PetOperationOtherArg, PetOperationOtherRes)

static KKSG::ErrorCode DoPairRide(Role* role, Role* otherrole, bool isinitiative)
{
	if (!GetGlobalConfig().IsOpenPairPet)
	{
		return KKSG::ERR_SYS_NOTOPEN;
	}
	///> 自己的条件(当前不处于骑乘状态)
	if (!role->Get<CRoleSystem>()->IsSystemOpened(SYS_PET))
	{
		return KKSG::ERR_PAIRPET_SELF;
	}
	if (!role->Get<PetSys>()->CanPairRideSelf())
	{
		return KKSG::ERR_PAIRPET_SELF;
	}
	///> 别人的条件(别人骑了双人坐骑，并且有空位)
	if (!otherrole->Get<PetSys>()->CanPairRideOther(isinitiative))
	{
		return KKSG::ERR_PAIRPET_OTHER;
	}
 
	if (!role->StateCanChangeTo(KKSG::OutLook_RidePetCopilot))
	{
		return KKSG::ERR_PAIRPET_SELF;
	}
	// role can see otherrole
	bool cansee = false;
	std::vector<Role*> roles;
	role->GetCurrScene()->GetRolesCanSee(role, roles);
	for (auto i = roles.begin(); i != roles.end(); ++i)
	{
		if (*i == otherrole)
		{
			cansee = true;
			break;
		}
	}
	if (!cansee)
	{
		return KKSG::ERR_PAIRPET_OUTRANGE;
	}

	bool canseeother = false;
	std::vector<Role*> rolesother;
	otherrole->GetCurrScene()->GetRolesCanSee(otherrole, rolesother);
	for (auto i = rolesother.begin(); i != rolesother.end(); ++i)
	{
		if (*i == role)
		{
			canseeother = true;
			break;
		}
	}
	if (!canseeother)
	{
		return KKSG::ERR_PAIRPET_OUTRANGE;
	}

 	///> 设置状态
	otherrole->Get<PetSys>()->SetPairRide(role->GetID(), false);
	role->Get<PetSys>()->SetPairRide(otherrole->GetID(), true);

	role->SetState(KKSG::OutLook_RidePetCopilot, otherrole->Get<PetSys>()->GetFollowConfId(), false, otherrole->GetID());

	role->GetCurrScene()->UpdateBindRole(role, otherrole);

	RoleOutLook outlook(role);
	outlook.Fill(SUMMARY_STATE, NULL);
	outlook.BroadCast();

	return KKSG::ERR_SUCCESS;
}

void RpcC2G_PetOperationOther::OnCall(const PetOperationOtherArg &roArg, PetOperationOtherRes &roRes)
{
	Role* role = RoleManager::Instance()->FindBySession(m_sessionID);
	if (NULL == role)
	{
		roRes.set_errorcode(KKSG::ERR_INVALID_REQUEST);
		return;
	}
	if (NULL == role->GetCurrScene() || role->GetCurrScene()->GetSceneType() != KKSG::SCENE_HALL)
	{
		roRes.set_errorcode(KKSG::ERR_PAIRPET_SCENEINVALID);
		return;
	}
	UINT64 otherroleid = roArg.otherroleid();
	Role* otherrole = RoleManager::Instance()->FindByRoleID(otherroleid);
	if (NULL == otherrole)
	{
		roRes.set_errorcode(KKSG::ERR_PAIRPET_OTHERNOTIN);
		return;
	}

	if (role == otherrole)
	{
		roRes.set_errorcode(KKSG::ERR_FAILED);
		return;
	}

	KKSG::ErrorCode errorcode = KKSG::ERR_FAILED;
	switch(roArg.op())
	{
	case KKSG::InvitePetPairRide:
		{
			if (!role->Get<PetSys>()->CanPairRideOther(true))
			{
				break;
			}
			KKSG::PetInviteInfo invite;
			invite.set_roleid(role->GetID());
			invite.set_rolename(role->GetName());
			invite.set_ppt((UINT32)role->GetAttr(TOTAL_POWERPOINT));
			invite.set_profession(role->GetProfession());
			invite.set_petuid(role->Get<PetSys>()->GetFollowId());
			invite.set_petconfigid(role->Get<PetSys>()->GetFollowConfId());
			invite.set_petppt(role->Get<PetSys>()->GetFollowPetPPT());

			if (!otherrole->Get<PetSys>()->PushPairRideInvite(invite))
			{
				errorcode = KKSG::ERR_PAIRPET_INVITEREPEAT;
				break;
			}
			UINT32 allcount = otherrole->Get<PetSys>()->GetPairRideInviteCount();
			PtcG2C_PetInviteNtf ntf;
			ntf.m_Data.mutable_info()->CopyFrom(invite);
			ntf.m_Data.set_allcount(allcount);
			otherrole->Send(ntf);

			errorcode = KKSG::ERR_SUCCESS;

			break;
		}
		///> 同意别人邀请
	case KKSG::AgreePetPairRide:
		{
			bool isfind = false;
			std::deque<KKSG::PetInviteInfo>* inviteinfo = role->Get<PetSys>()->GetPairRideInvite();
			if (NULL != inviteinfo)
			{
				for (auto i = inviteinfo->begin(); i != inviteinfo->end();)
				{
					if (i->roleid() == otherroleid)
					{
						isfind = true;
						inviteinfo->erase(i++);
						break;
					}
					else
					{
						++i;
					}
				}
			}
			if (isfind)
			{
				errorcode = DoPairRide(role, otherrole, true);
			}
			else
			{
				errorcode = KKSG::ERR_PAIRPET_INVITEINVALID;
			}
			break;
		}
		///> 请求别人骑乘
	case KKSG::DoPetPairRide:
		{
			errorcode = DoPairRide(role, otherrole, false);
			break;
		}
	default:
		break;
	}
	roRes.set_errorcode(errorcode);
}

void RpcC2G_PetOperationOther::OnDelayReplyRpc(const PetOperationOtherArg &roArg, PetOperationOtherRes &roRes, const CUserData &roUserData)
{
}
