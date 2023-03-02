#include "pch.h"
#include "ms_commondef_handler.h"
#include "util/jsonutil.h"
#include "role/bag.h"

IDIP_HANDLER_MODIFY_IMPLEMETION(DelItem)
INT32 CMSIdipDelItemHandler::HandlerRequest(const rapidjson::Value& roHeadJson, const rapidjson::Value& roBodyJson, CIdipReply& roReply)
{
	UINT64 roleid = JsonUtil::GetJsonUint64(roBodyJson, "RoleId");
	m_uItemID	= JsonUtil::GetJsonUint(roBodyJson, "ItemId");
	m_nNum	= JsonUtil::GetJsonInt(roBodyJson, "ItemNum");
	szOpenId = JsonUtil::GetJsonString(roBodyJson, "OpenId");

	if (!CRoleSummaryMgr::Instance()->IsValidAcountRole(szOpenId, roleid))
	{
		roReply.m_nErrCode = IDIP_ERR_PLAYER_NOT_EXIST;
		roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
		return IDIP_SERVER_NONE;
	}
	SetRoleID(roleid);

	if(m_nNum <= 0 || m_uItemID == DIAMOND)
	{
		roReply.m_nErrCode = IDIP_ERR_INVALID_REQ_NUM;
		roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
		return IDIP_SERVER_NONE;
	}
	CRole* pRole = CRoleManager::Instance()->GetByRoleID(roleid);
	if (NULL != pRole)//玩家在线
	{
		return IDIP_SERVER_GS;
	}
	else
	{
		return IDIP_SERVER_MS;
	}
}
//修改数据
bool CMSIdipDelItemHandler::ModifyMsData(CUserData& roUserData)
{
	RoleAllInfo* pInfo = (RoleAllInfo*)roUserData.m_pUserPtr;
	CBag data(pInfo);
	data.ReduceItem(m_uItemID, abs(m_nNum));
	data.Save();
	return true;
}

void CMSIdipDelItemHandler::IdipLog()
{
	rapidjson::Document doc;
	std::string s = GetStrJson();
	if (doc.Parse<0>(GetStrJson().c_str()).HasParseError())
	{
		return ;
	}
	TIdipDelItemLog oLog("IdipDelItemFlow");
	oLog.szOpenId = szOpenId;
	oLog.m_uRoleId = GetRoleID();
	oLog.m_uItemID = m_uItemID;
	oLog.m_nNum = m_nNum;
	oLog.Do(doc["head"], doc["body"]);
}
