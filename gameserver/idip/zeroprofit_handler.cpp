#include "pch.h"
#include "commondef_handler.h"
#include "util/jsonutil.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "idip/idiprecord.h"

bool CIdipZeroProfitHandler::Process(const rapidjson::Value& roHeadJson, const rapidjson::Value& roBodyJson, CIdipReply& roReply)
{
	UINT64 qwRoleID = JsonUtil::GetJsonUint64(roBodyJson, "RoleId");
	INT32 iBanTime = JsonUtil::GetJsonInt(roBodyJson, "ZeroProfitTime");
	std::string strReason = JsonUtil::GetJsonString(roBodyJson, "Reason");
	Role* pRole = RoleManager::Instance()->FindByRoleIDOrNickID(qwRoleID);
	if (NULL == pRole)
	{
		roReply.m_nErrCode = IDIP_ERR_PLAYER_NOT_EXIST;
		roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
		return true;
	}
	KKSG::IdipPunishData oData;
	oData.set_type(KKSG::PUNISH_USER_ZERO_PROFIT);
	oData.set_punishtime(INT32(time(NULL)));
	oData.set_bantime(iBanTime);
	oData.set_reason(strReason);
	pRole->Get<CIdipRecord>()->SetIdipPunishData(oData);
	roReply.m_nErrCode = IDIP_ERR_SUCCESS;
	roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
	return true;
}
