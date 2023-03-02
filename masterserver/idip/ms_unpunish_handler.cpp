#include "pch.h"
#include "ms_commondef_handler.h"
#include "util/jsonutil.h"
#include <time.h>
#include "network/dblink.h"
#include "account/punishmgr.h"
#include "account/ptcm2d_savepunishdata.h"

INT32 CMSIdipUnPunishHandler::HandlerRequest(const rapidjson::Value& roHeadJson, const rapidjson::Value& roBodyJson, CIdipReply& roReply)
{
	std::string strOpenId = JsonUtil::GetJsonString(roBodyJson, "OpenId");
	UINT64 qwRoleID = JsonUtil::GetJsonUint64(roBodyJson, "RoleId");
	if (!CRoleSummaryMgr::Instance()->IsValidAcountRole(strOpenId, qwRoleID))
	{
		roReply.m_nErrCode = IDIP_ERR_PLAYER_NOT_EXIST;
		roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
		return IDIP_SERVER_NONE;
	}
	SetRoleID(qwRoleID);

	CPunishMgr::Instance()->UnDoPunishUser(qwRoleID, KKSG::PUNISH_USER_LOGIN);

	TIdipRelievePunish oLog("TIdipDoUnbanUsrFlow");
	oLog.m_uRoleId = GetRoleID();
	oLog.m_strOpenId = strOpenId;
	oLog.Do(roHeadJson, roBodyJson);

	roReply.m_nErrCode = IDIP_ERR_SUCCESS;
	roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
	return IDIP_SERVER_NONE;
}
