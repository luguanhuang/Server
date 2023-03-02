#include "pch.h"
#include "ms_commondef_handler.h"
#include "util/jsonutil.h"
#include "table/ItemConfigMgr.h"

IDIP_REPLY_FROMGS_IMPLEMETION(ItemChange)
bool CMSIdipItemChangeHandler::CheckItemChange(UINT32 Count, std::vector<ItemDesc>& list)
{
	if (Count == 0 || Count != list.size())
	{
		return false;
	}

	for (UINT32 i = 0; i < list.size(); i ++)
	{
		if (CItemConfig::Instance()->GetItemConf(list[i].itemID) == NULL || list[i].itemID == DIAMOND)
		{
			return false;
		}
	}
	return true;
}

INT32 CMSIdipItemChangeHandler::HandlerRequest(const rapidjson::Value& roHeadJson, const rapidjson::Value& roBodyJson, CIdipReply& roReply)
{
	UINT64 roleid = JsonUtil::GetJsonUint64(roBodyJson, "RoleId");
	m_strOpenId = JsonUtil::GetJsonString(roBodyJson, "OpenId");

	if (!CRoleSummaryMgr::Instance()->IsValidAcountRole(m_strOpenId, roleid))
	{
		roReply.m_nErrCode = IDIP_ERR_PLAYER_NOT_EXIST;
		roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
		return IDIP_SERVER_NONE;
	}
	SetRoleID(roleid);

	CRole* pRole = CRoleManager::Instance()->GetByRoleID(roleid);
	if (NULL == pRole || pRole->IsInCrossGs())
	{
		roReply.m_nErrCode = IDIP_ERR_UNKNOWN;
		roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
		return IDIP_SERVER_NONE;
	}
	m_SItemListCount = JsonUtil::GetJsonUint(roBodyJson, "SItemList_count");
	m_DItemListCount = JsonUtil::GetJsonUint(roBodyJson, "DItemList_count");
	const rapidjson::Value& roMember = JsonUtil::GetJsonValue(roBodyJson, "SItemList");
	const rapidjson::Value& roDMember = JsonUtil::GetJsonValue(roBodyJson, "DItemList");
	if (!GetItemChangeList(roMember, m_vSItemList) || !GetItemChangeList(roDMember, m_vDItemList))
	{
		roReply.m_nErrCode = IDIP_ERR_INVALID_PARA;
		roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
		return IDIP_SERVER_NONE;
	}

	if (!CheckItemChange(m_SItemListCount, m_vSItemList) || !CheckItemChange(m_DItemListCount, m_vDItemList))
	{
		LogError("invalid itemid role=%llu,openid=%s", roleid, m_strOpenId.c_str());
		roReply.m_nErrCode = IDIP_ERR_INVALID_PARA;
		roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
		return IDIP_SERVER_NONE;
	}
	return IDIP_SERVER_GS;
}

void CMSIdipItemChangeHandler::DoLog(std::vector<ItemDesc>& list, INT32 op)
{
	rapidjson::Document doc;
	if (doc.Parse<0>(GetStrJson().c_str()).HasParseError())
	{
		return ;
	}
	TIdipItemChange oLog("IdipItemChange");
	oLog.m_RoleId = GetRoleID();
	oLog.m_strOpenId = m_strOpenId;
	for (auto it = list.begin(); it != list.end(); it ++)
	{
		oLog.m_ItemId = it->itemID;
		oLog.m_ItemNum = it->itemCount;
		oLog.m_Op = op;
		oLog.Do(doc["head"], doc["body"]);
	}
}

void CMSIdipItemChangeHandler::IdipLog()
{
	DoLog(m_vSItemList, 0);
	DoLog(m_vDItemList, 1);
}
