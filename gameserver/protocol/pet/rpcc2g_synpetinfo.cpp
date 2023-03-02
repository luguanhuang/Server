#include "pch.h"
#include "pet/rpcc2g_synpetinfo.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "unit/pet.h"

// generate by ProtoGen at date: 2016/8/16 18:11:22

RPC_SERVER_IMPLEMETION(RpcC2G_SynPetInfo, SynPetInfoArg, SynPetInfoRes)

void RpcC2G_SynPetInfo::OnCall(const SynPetInfoArg &roArg, SynPetInfoRes &roRes)
{
	Role* pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (pRole == NULL)
	{
		SSWarn<<"pRole is NULL, session id:"<<m_sessionID<<END;
		return;
	}

	PetSys* pPetSys = pRole->Get<PetSys>();
	if (pPetSys == NULL)
	{
		SSWarn<<"pPetSyS is NULL"<<END;
		return;
	}

	Pet* pPet = pPetSys->GetPet(roArg.uid()); 
	if (pPet != NULL)
	{
		pPet->Update();
		roRes.set_mood(pPet->mood);
		roRes.set_hungry(pPet->hungry);
	}
}

void RpcC2G_SynPetInfo::OnDelayReplyRpc(const SynPetInfoArg &roArg, SynPetInfoRes &roRes, const CUserData &roUserData)
{
}
