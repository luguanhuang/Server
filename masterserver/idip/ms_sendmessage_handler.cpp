#include "pch.h"
#include "ms_commondef_handler.h"
#include "util/jsonutil.h"
#include "role/rolesummarymgr.h"
#include "common/ptcm2c_notifyidipmessagems.h"

IDIP_HANDLER_MODIFY_IMPLEMETION(SendMsg)


INT32 CMSIdipSendMsgHandler::HandlerRequest(const rapidjson::Value& roHeadJson, const rapidjson::Value& roBodyJson, CIdipReply& roReply)
{
	szOpenId = JsonUtil::GetJsonString(roBodyJson, "OpenId");
	UINT64 uRoleId = JsonUtil::GetJsonUint64(roBodyJson, "RoleId");
	if (!CRoleSummaryMgr::Instance()->IsValidAcountRole(szOpenId, uRoleId))
	{
		roReply.m_nErrCode = IDIP_ERR_PLAYER_NOT_EXIST;
		roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
		return IDIP_SERVER_NONE;
	}
	SetRoleID(uRoleId);

	szMailContent = JsonUtil::GetJsonString(roBodyJson, "Message");

	CRole* pRole = CRoleManager::Instance()->GetByRoleID(uRoleId);
	if (NULL == pRole)
	{
		return IDIP_SERVER_MS;
	}
	
	PtcM2C_NotifyIdipMessageMs ptc;
	ptc.m_Data.set_message(szMailContent);
	pRole->Send(ptc);
	roReply.m_nErrCode = IDIP_ERR_SUCCESS;
	roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
	return IDIP_SERVER_NONE;
}
bool CMSIdipSendMsgHandler::ModifyMsData(CUserData& roUserData)
{
	RoleAllInfo* pInfo = (RoleAllInfo*)roUserData.m_pUserPtr;
	KKSG::IdipMessage* pMess =  pInfo->mutable_idiprecord()->mutable_mess();
	pMess->set_message(szMailContent);
	return true;
}
void CMSIdipSendMsgHandler::IdipLog()
{

}