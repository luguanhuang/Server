#include "pch.h"
#include "commondef_handler.h"
#include "util/jsonutil.h"
#include "gamelogic/rolesummarymgr.h"
#include "gamelogic/rolesummary.h"
#include "unit/rolemanager.h"

bool CIdipUpdateNameHandler::Process(const rapidjson::Value& roHeadJson, const rapidjson::Value& roBodyJson, CIdipReply& roReply)
{
	std::string strOpenId = JsonUtil::GetJsonString(roBodyJson, "OpenId");
	UINT64 qwRoleID = JsonUtil::GetJsonUint64(roBodyJson, "RoleId");
	std::string strName = JsonUtil::GetJsonString(roBodyJson, "Name");
	Role* pRole = RoleManager::Instance()->FindByRoleIDOrNickID(qwRoleID);
	if (NULL == pRole)
	{
		roReply.m_nErrCode = IDIP_ERR_PLAYER_NOT_EXIST;
		roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
		return true;
	}
	pRole->GetRoleAllInfoPtr()->mutable_brief()->set_name(strName);

	RoleSummary* summary = RoleSummaryMgr::Instance()->GetRoleSummary(pRole->GetID());
	if (NULL != summary)
	{
		summary->SetName(strName);
		RoleSummaryMgr::Instance()->UpdateRoleSummary(pRole, SUMMARY_NAME);
	}
	roReply.m_nErrCode = IDIP_ERR_SUCCESS;
	roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
	return true;
}
