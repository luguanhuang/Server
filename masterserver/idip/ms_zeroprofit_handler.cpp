#include "pch.h"
#include "ms_commondef_handler.h"
#include "util/jsonutil.h"


IDIP_HANDLER_MODIFY_IMPLEMETION(ZeroProfit)

INT32 CMSIdipZeroProfitHandler::HandlerRequest(const rapidjson::Value& roHeadJson, const rapidjson::Value& roBodyJson, CIdipReply& roReply)
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
	szMailContent = JsonUtil::GetJsonString(roBodyJson, "Reason");
	m_nNum = JsonUtil::GetJsonInt(roBodyJson, "ZeroProfitTime");
	if (m_nNum < 0)
	{
		roReply.m_nErrCode = IDIP_ERR_INVALID_PARA;
		roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
		return IDIP_SERVER_NONE;
	}
	CRole* pRole = CRoleManager::Instance()->GetByRoleID(uRoleId);
	if (NULL == pRole)
	{
		return IDIP_SERVER_MS;
	}
	return IDIP_SERVER_GS;
}
bool CMSIdipZeroProfitHandler::ModifyMsData(CUserData& roUserData)
{
	RoleAllInfo* pInfo = (RoleAllInfo*)roUserData.m_pUserPtr;
	KKSG::IdipPunishData* pData = NULL;
	for (INT32 i = 0; i < pInfo->idiprecord().punishinfo_size(); ++ i)
	{
		if (pInfo->idiprecord().punishinfo(i).type() == KKSG::PUNISH_USER_ZERO_PROFIT)
		{
			pData = pInfo->mutable_idiprecord()->mutable_punishinfo(i);
			break;
		}
	}
	if (NULL == pData)
	{
		pData = pInfo->mutable_idiprecord()->add_punishinfo();
	}
	pData->set_type(KKSG::PUNISH_USER_ZERO_PROFIT);
	pData->set_punishtime(INT32(time(NULL)));
	pData->set_bantime(INT32(m_nNum));
	pData->set_reason(szMailContent);
	return true;
}

void CMSIdipZeroProfitHandler::IdipLog()
{
	rapidjson::Document doc;
	std::string s = GetStrJson();
	if (doc.Parse<0>(GetStrJson().c_str()).HasParseError())
	{
		return ;
	}
	TIdipZeroProfit oLog("TIdipZeroProfitFlow");
	oLog.m_uRoleId = GetRoleID();
	oLog.m_strOpenId = szOpenId;
	oLog.m_iZeroProfitTime = m_nNum;
	oLog.m_strReason = szMailContent;
	oLog.Do(doc["head"], doc["body"]);
}
