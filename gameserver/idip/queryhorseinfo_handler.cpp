#include "pch.h"
#include "commondef_handler.h"
#include "util/jsonutil.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "unit/pet.h"
#include "unit/petconfigmgr.h"

bool CIdipQueryHorseInfoHandler::Process(const rapidjson::Value& roHeadJson, const rapidjson::Value& roBodyJson, CIdipReply& roReply)
{
	std::string strOpenId = JsonUtil::GetJsonString(roBodyJson, "OpenId");
	UINT64 uRoleId = JsonUtil::GetJsonUint64(roBodyJson, "RoleId");

	Role* pRole = RoleManager::Instance()->FindByRoleIDOrNickID(uRoleId);
	if (NULL == pRole)
	{
		roReply.m_nErrCode = IDIP_ERR_PLAYER_NOT_EXIST;
		roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
		return true;
	}
	JsonBuilder oBuilder;
	rapidjson::Value oList(rapidjson::kArrayType);
	std::map<UINT64, Pet> vList = pRole->Get<PetSys>()->GetAllPet();
	for (auto i = vList.begin(); i != vList.end(); i ++)
	{
		UINT32 Quality = 0;
		std::string strName = "null";
		auto pConf = PetConfigMgr::Instance()->GetPetInfoData(i->second.petid);
		if (pConf)
		{
			Quality = pConf->quality;
			strName = pConf->name;
		}
		rapidjson::Value oPet(rapidjson::kObjectType);
		oPet.AddMember("OpenId", strOpenId, oBuilder.Allocator());
		oPet.AddMember("UId", pRole->GetNickId(), oBuilder.Allocator());
		oPet.AddMember("RoleId", ToString(pRole->GetRoleID()), oBuilder.Allocator());
		oPet.AddMember("NickName", UrlEncode(pRole->GetName()), oBuilder.Allocator());
		oPet.AddMember("HorseName", UrlEncode(strName), oBuilder.Allocator());
		oPet.AddMember("HorseLevel", i->second.level, oBuilder.Allocator());
		oPet.AddMember("HorseQual", ToString(Quality), oBuilder.Allocator());
		oPet.AddMember("HorseFeel", i->second.mood, oBuilder.Allocator());
		oPet.AddMember("IsAct", pRole->Get<PetSys>()->GetFightId()==i->second.uid?0:1, oBuilder.Allocator());
		oPet.AddMember("ExpNum", i->second.power, oBuilder.Allocator());
		oPet.AddMember("HorseUid", ToString(i->second.uid), oBuilder.Allocator());
		oPet.AddMember("HorseId", i->second.petid, oBuilder.Allocator());
		oList.PushBack(oPet, oBuilder.Allocator());
	}
	oBuilder.AddMember_Int("Result", KKSG::ERR_SUCCESS);
	oBuilder.AddMember_Uint("HorseList_count", vList.size());
	oBuilder.AddMember_Value("HorseList", oList);

	roReply.m_nErrCode = IDIP_ERR_SUCCESS;
	roReply.m_strResult = oBuilder.GetString();
	return true;
}
