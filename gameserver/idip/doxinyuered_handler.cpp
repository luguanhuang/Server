#include "pch.h"
#include "commondef_handler.h"
#include "util/jsonutil.h"
#include "unit/rolemanager.h"
#include "idiprecord.h"
#include "gamelogic/rolemisc.h"
bool CIdipDoXinyueRedHandler::Process(const rapidjson::Value& roHeadJson, const rapidjson::Value& roBodyJson, CIdipReply& roReply)
{
	std::string strOpenId = JsonUtil::GetJsonString(roBodyJson, "OpenId");
	UINT64 qwRoleID = JsonUtil::GetJsonUint64(roBodyJson, "RoleId");
	Role* pRole = RoleManager::Instance()->FindByRoleIDOrNickID(qwRoleID);
	if (NULL == pRole)
	{
		roReply.m_nErrCode = IDIP_ERR_PLAYER_NOT_EXIST;
		roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
		return true;
	}
	pRole->Get<CIdipRecord>()->SetXinYueHint(true);
	pRole->Get<RoleMisc>()->CheckXinYueHint();
	roReply.m_nErrCode = IDIP_ERR_SUCCESS;
	roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
	return true;
}

