#include "pch.h"
#include "commondef_handler.h"
#include "util/jsonutil.h"
#include "unit/rolemanager.h"
#include "gamelogic/payv2Record.h"

bool CIdipDiamondConsumeToGameHandler::Process(const rapidjson::Value& roHeadJson, const rapidjson::Value& roBodyJson, CIdipReply& roReply)
{
	UINT64 roleid = JsonUtil::GetJsonUint64(roBodyJson, "RoleId");
	INT32 num	= JsonUtil::GetJsonInt(roBodyJson, "Diamond");
	std::string strOpenId = JsonUtil::GetJsonString(roBodyJson, "OpenId");
	Role* pRole = RoleManager::Instance()->FindByRoleIDOrNickID(roleid);
	if (NULL == pRole)
	{
		roReply.m_nErrCode = IDIP_ERR_PLAYER_NOT_EXIST;
		roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
		return true;
	}
	pRole->Get<CPayV2Record>()->AddScore(num);
	roReply.m_nErrCode = IDIP_ERR_SUCCESS;
	roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
	return true;
}
