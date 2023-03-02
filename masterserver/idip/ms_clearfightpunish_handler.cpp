#include "pch.h"
#include "ms_commondef_handler.h"
#include "util/jsonutil.h"
#include "report/reportmgr.h"
#include "report/rolereport.h"

INT32 CMSIdipClearFightPunishHandler::HandlerRequest(const rapidjson::Value& roHeadJson, const rapidjson::Value& roBodyJson, CIdipReply& roReply)
{
	std::string strOpenId = JsonUtil::GetJsonString(roBodyJson, "OpenId");
	UINT64 roleid = JsonUtil::GetJsonUint64(roBodyJson, "RoleId");
	if (!CRoleSummaryMgr::Instance()->IsValidAcountRole(strOpenId, roleid))
	{
		roReply.m_nErrCode = IDIP_ERR_PLAYER_NOT_EXIST;
		roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
		return IDIP_SERVER_NONE;
	}
	SetRoleID(roleid);
	RoleReportInfo* pReportInfo = ReportMgr::Instance()->GetRoleReportInfo(roleid);
	if (pReportInfo && pReportInfo->GetBanTime(KKSG::SCENE_HEROBATTLE))
	{
		pReportInfo->ResetBanTime(KKSG::SCENE_HEROBATTLE);
	}
	else
	{
		roReply.m_nErrCode = IDIP_ERR_UNKNOWN;
		roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
		return IDIP_SERVER_NONE;
	}
	roReply.m_nErrCode = IDIP_ERR_SUCCESS;
	roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);

	TIdipClearFightPunishFlow oLog("TIdipClearFightPunishFlow");
	oLog.m_RoleId = roleid;
	oLog.m_strOpenId = strOpenId;
	oLog.Do(roHeadJson, roBodyJson);
	return IDIP_SERVER_NONE;
}
