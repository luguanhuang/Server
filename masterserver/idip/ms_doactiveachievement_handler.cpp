#include "pch.h"
#include "ms_commondef_handler.h"
#include "util/jsonutil.h"
#include "event/eventmgr.h"

INT32 CMSIdipDoActiveAchievementHandler::HandlerRequest(const rapidjson::Value& roHeadJson, const rapidjson::Value& roBodyJson, CIdipReply& roReply)
{
	std::string strOpenId = JsonUtil::GetJsonString(roBodyJson, "OpenId");
	UINT64 qwRoleId = JsonUtil::GetJsonUint64(roBodyJson, "RoleId");
	if (!CRoleSummaryMgr::Instance()->IsValidAcountRole(strOpenId, qwRoleId))
	{
		roReply.m_nErrCode = IDIP_ERR_PLAYER_NOT_EXIST;
		roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
		return IDIP_SERVER_NONE;
	}
	SetRoleID(qwRoleId);

	UINT32 achId = JsonUtil::GetJsonUint(roBodyJson, "AchievementId");
	UINT32 type = JsonUtil::GetJsonUint(roBodyJson, "Type");
	if (!achId || type > 1)
	{
		roReply.m_nErrCode = IDIP_ERR_PLAYER_NOT_EXIST;
		roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
		return IDIP_SERVER_NONE;
	}
	if (type == 0)
	{
		EventMgr::Instance()->AddAch(qwRoleId, achId);
	}
	else
	{
		EventMgr::Instance()->DelAch(qwRoleId, achId);
	}
	roReply.m_nErrCode = IDIP_ERR_SUCCESS;
	roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
	return IDIP_SERVER_NONE;
}
