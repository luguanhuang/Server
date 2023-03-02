#include "pch.h"
#include "commondef_handler.h"
#include "util/jsonutil.h"
#include "idip/idiprecord.h"
#include "unit/role.h"
#include "unit/rolemanager.h"

bool CIdipRelievePunishHandler::Process(const rapidjson::Value& roHeadJson, const rapidjson::Value& roBodyJson, CIdipReply& roReply)
{
	UINT64 qwRoleID = JsonUtil::GetJsonUint64(roBodyJson, "RoleId");
	bool bIsRelieveZeroProfit = JsonUtil::GetJsonUint(roBodyJson, "RelieveZeroProfit")==1?true:false;
	bool bIsRelievePlayAll = JsonUtil::GetJsonUint(roBodyJson, "RelievePlayAll")==1?true:false;
	Role* pRole = RoleManager::Instance()->FindByRoleIDOrNickID(qwRoleID);
	if (NULL == pRole)
	{
		roReply.m_nErrCode = IDIP_ERR_PLAYER_NOT_EXIST;
		roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
		return true;
	}
	if (bIsRelieveZeroProfit)
	{
		pRole->Get<CIdipRecord>()->UnDoIdipPunishData(KKSG::PUNISH_USER_ZERO_PROFIT);
	}
	
	if (bIsRelievePlayAll)
	{
		pRole->Get<CIdipRecord>()->UnDoIdipPunishData(KKSG::PUNISH_USER_DAILY_PLAY);
		pRole->Get<CIdipRecord>()->UnDoIdipPunishData(KKSG::PUNISH_USER_MULTI_ACTIVITY);
	}
	roReply.m_nErrCode = IDIP_ERR_SUCCESS;
	roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
	return true;
}
