#include "pch.h"
#include "ms_commondef_handler.h"
#include "util/jsonutil.h"
#include "event/eventmgr.h"
#include "role/bag.h"

IDIP_HANDLER_MODIFY_IMPLEMETION(NameAct)
INT32 CMSIdipNameActHandler::HandlerRequest(const rapidjson::Value& roHeadJson, const rapidjson::Value& roBodyJson, CIdipReply& roReply)
{
	std::string strOpenId = JsonUtil::GetJsonString(roBodyJson, "OpenId");
	std::string  strNickID = JsonUtil::GetJsonString(roBodyJson, "UId");
	UINT64 NickID = strNickID.empty()?0:convert<UINT64>(strNickID);
	std::string strNameId = JsonUtil::GetJsonString(roBodyJson, "NameId");
	m_uItemID = strNameId.empty()?0:convert<UINT32>(strNameId);
	UINT32 isActivity = JsonUtil::GetJsonUint(roBodyJson, "IsActivity");

	if (!NickID || !m_uItemID)
	{
		roReply.m_nErrCode = IDIP_ERR_INVALID_PARA;
		roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
		return IDIP_SERVER_NONE;
	}
	if (!CRoleSummaryMgr::Instance()->IsValidAcountRole(strOpenId, NickID))
	{
		roReply.m_nErrCode = IDIP_ERR_PLAYER_NOT_EXIST;
		roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
		return IDIP_SERVER_NONE;
	}
	SetRoleID(NickID);
	if (isActivity == 0)//激活
	{
		EventMgr::Instance()->AddDes(NickID, m_uItemID);
	}
	else if (isActivity == 1)//取消
	{
		CRole* pRole = CRoleManager::Instance()->GetByRoleID(NickID);
		if (NULL == pRole)//不在线ms读取数据扣除;
		{
			return IDIP_SERVER_MS;
		}
		return IDIP_SERVER_GS;
	}
	else
	{
		roReply.m_nErrCode = IDIP_ERR_INVALID_PARA;
		roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
		return IDIP_SERVER_NONE;
	}
	roReply.m_nErrCode = IDIP_ERR_SUCCESS;
	roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
	return IDIP_SERVER_NONE;
}

bool CMSIdipNameActHandler::ModifyMsData(CUserData& roUserData)
{
	RoleAllInfo* pInfo = (RoleAllInfo*)roUserData.m_pUserPtr;
	CBag oData(pInfo);
	oData.DelDesignationID(m_uItemID);
	oData.Save();
	EventMgr::Instance()->DelDes(GetRoleID(), m_uItemID);
	return true;
}

void CMSIdipNameActHandler::IdipLog()
{

}
