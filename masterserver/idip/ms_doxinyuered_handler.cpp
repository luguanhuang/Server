#include "pch.h"
#include "ms_commondef_handler.h"
#include "util/jsonutil.h"

IDIP_HANDLER_MODIFY_IMPLEMETION(DoXinyueRed)
INT32 CMSIdipDoXinyueRedHandler::HandlerRequest(const rapidjson::Value& roHeadJson, const rapidjson::Value& roBodyJson, CIdipReply& roReply)
{
	szOpenId = JsonUtil::GetJsonString(roBodyJson, "OpenId");
	UINT64 roleid = JsonUtil::GetJsonUint64(roBodyJson, "RoleId");
	if (!CRoleSummaryMgr::Instance()->IsValidAcountRole(szOpenId, roleid))
	{
		roReply.m_nErrCode = IDIP_ERR_PLAYER_NOT_EXIST;
		roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
		return IDIP_SERVER_NONE;
	}
	SetRoleID(roleid);
	CRole* pRole = CRoleManager::Instance()->GetByRoleID(roleid);
	if (NULL == pRole)//不在线ms读取数据扣除;
	{
		return IDIP_SERVER_MS;
	}
	return IDIP_SERVER_GS;
}
bool CMSIdipDoXinyueRedHandler::ModifyMsData(CUserData& roUserData)
{
	RoleAllInfo* pInfo = (RoleAllInfo*)roUserData.m_pUserPtr;
	pInfo->mutable_idiprecord()->set_xinyue_hint(true);
	return true;
}
void CMSIdipDoXinyueRedHandler::IdipLog()
{

}
