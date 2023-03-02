#include "pch.h"
#include "commondef_handler.h"
#include "util/jsonutil.h"
#include "idip/idiprecord.h"
#include "unit/rolemanager.h"

bool CIdipBanPlayHandler::Process(const rapidjson::Value& roHeadJson, const rapidjson::Value& roBodyJson, CIdipReply& roReply)
{
	UINT64 qwRoleID = JsonUtil::GetJsonUint64(roBodyJson, "RoleId");
	INT32 iBanTime = JsonUtil::GetJsonInt(roBodyJson, "BanTime");
	std::string strText = JsonUtil::GetJsonString(roBodyJson, "PromptText");
	UINT32 uType = (GetCmd()==IDIP_AQ_DO_BAN_ALL_PLAY_REQ)?BanPlayTypeALL:JsonUtil::GetJsonUint(roBodyJson, "BanType");
	Role* pRole = RoleManager::Instance()->FindByRoleIDOrNickID(qwRoleID);
	if (NULL == pRole)
	{
		roReply.m_nErrCode = IDIP_ERR_PLAYER_NOT_EXIST;
		roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
		return true;
	}

	KKSG::IdipPunishData oData;
	oData.set_punishtime(INT32(time(NULL)));
	oData.set_bantime(iBanTime);
	oData.set_reason(strText);

	switch (uType)
	{
	case BanPlayTypeDailyPlay:
		oData.set_type(KKSG::PUNISH_USER_DAILY_PLAY);
		pRole->Get<CIdipRecord>()->SetIdipPunishData(oData);
		break;
	case BanPlayTypeMutilActivity:
		oData.set_type(KKSG::PUNISH_USER_MULTI_ACTIVITY);
		pRole->Get<CIdipRecord>()->SetIdipPunishData(oData);
		break;
	case BanPlayTypeALL:
		oData.set_type(KKSG::PUNISH_USER_DAILY_PLAY);
		pRole->Get<CIdipRecord>()->SetIdipPunishData(oData);
		oData.set_type(KKSG::PUNISH_USER_MULTI_ACTIVITY);
		pRole->Get<CIdipRecord>()->SetIdipPunishData(oData);
		break;
	default:
		break;
	}
	roReply.m_nErrCode = IDIP_ERR_SUCCESS;
	roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
	return true;
}