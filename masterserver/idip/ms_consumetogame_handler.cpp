#include "pch.h"
#include "ms_commondef_handler.h"
#include "util/jsonutil.h"

IDIP_HANDLER_MODIFY_IMPLEMETION(DiamondConsumeToGame)
INT32 CMSIdipDiamondConsumeToGameHandler::HandlerRequest(const rapidjson::Value& roHeadJson, const rapidjson::Value& roBodyJson, CIdipReply& roReply)
{
	UINT64 roleid = JsonUtil::GetJsonUint64(roBodyJson, "RoleId");
	m_nNum	= JsonUtil::GetJsonInt64(roBodyJson, "Diamond");
	szOpenId = JsonUtil::GetJsonString(roBodyJson, "OpenId");

	if (m_nNum <= 0)
	{
		roReply.m_nErrCode = IDIP_ERR_INVALID_REQ_NUM;
		roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
		return IDIP_SERVER_NONE;
	}

	if (!CRoleSummaryMgr::Instance()->IsValidAcountRole(szOpenId, roleid))
	{
		roReply.m_nErrCode = IDIP_ERR_PLAYER_NOT_EXIST;
		roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
		return IDIP_SERVER_NONE;
	}
	SetRoleID(roleid);
	CRole* poRole = CRoleManager::Instance()->GetByRoleID(roleid);
	if (NULL == poRole)
	{
		roReply.m_nErrCode = IDIP_ERR_ROLE_OFFLINE;
		roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
		return IDIP_SERVER_NONE;
	}
	LogInfo("openid=%s,roleid=%llu,score=%llu", szOpenId.c_str(), roleid, m_nNum);
	return IDIP_SERVER_GS;
}

bool CMSIdipDiamondConsumeToGameHandler::ModifyMsData(CUserData& roUserData)
{
	return true;
}

void CMSIdipDiamondConsumeToGameHandler::IdipLog()
{

}
