#include "pch.h"
#include "commondef_handler.h"
#include "util/jsonutil.h"
#include "unit/rolemanager.h"
#include "unit/role.h"
#include "gamelogic/rolefashion.h"
#include "table/ProfessionMgr.h"

bool CIdipQueryFashionHandler::Process(const rapidjson::Value& roHeadJson, const rapidjson::Value& roBodyJson, CIdipReply& roReply)
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

	std::vector<FashionInfo*> vFashion;
	pRole->Get<RoleFashion>()->GetAllFashionInfo(vFashion);
	JsonBuilder oBuilder;
	UINT32 FashionList_count = 0;
	rapidjson::Value oList(rapidjson::kArrayType);
	for (UINT32 i = 0; i < vFashion.size(); i ++)
	{
		if (vFashion[i] == NULL || !vFashion[i]->itemID)
		{
			continue;
		}
		std::string name = UrlEncode(ItemConfig::Instance()->GetItemName(vFashion[i]->itemID, ProfessionMgr::Instance()->GetBasicProfession(pRole->GetProfession())));
		rapidjson::Value item(rapidjson::kObjectType);
		item.AddMember("Position", pRole->Get<RoleFashion>()->GetBodyFashion(vFashion[i]->uid)?0:1, oBuilder.Allocator());
		item.AddMember("FashionId", vFashion[i]->itemID, oBuilder.Allocator());
		item.AddMember("FashionName", name.empty()?"null":name, oBuilder.Allocator());
		item.AddMember("FashionUId", ToString(vFashion[i]->uid), oBuilder.Allocator());
		oList.PushBack(item, oBuilder.Allocator());
		FashionList_count ++;
	}
	oBuilder.AddMember_Uint("FashionList_count", FashionList_count);
	oBuilder.AddMember_Value("FashionList", oList);

	roReply.m_nErrCode = IDIP_ERR_SUCCESS;
	roReply.m_strResult = oBuilder.GetString();
	return true;
}