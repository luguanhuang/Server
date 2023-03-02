#include "pch.h"
#include "commondef_handler.h"
#include "idip/idiperrdesc.h"
#include "util/jsonutil.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "define/itemdef.h"
#include "gamelogic/rolefashion.h"

bool CIdipQueryFashionComposeHandler::Process(const rapidjson::Value& roHeadJson, const rapidjson::Value& roBodyJson, CIdipReply& roReply)
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
	oBuilder.AddMember_Int("Num", pRole->Get<RoleFashion>()->GetComposeNum());
	oBuilder.AddMember_Int("SuccessNum", pRole->Get<RoleFashion>()->GetComposeSuccessNum());
	oBuilder.AddMember_Int("FailNum", pRole->Get<RoleFashion>()->GetComposeFailedNum());
	oBuilder.AddMember_Int("SNum", pRole->Get<RoleFashion>()->GetCompseSucessNumByQuality(FashionSQuality));
	oBuilder.AddMember_Int("ANum", pRole->Get<RoleFashion>()->GetCompseSucessNumByQuality(FashionAQuality));
	roReply.m_nErrCode = IDIP_ERR_SUCCESS;
	roReply.m_strResult = oBuilder.GetString();
	return true;
}
