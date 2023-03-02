#include "pch.h"
#include "ms_commondef_handler.h"
#include "util/jsonutil.h"
#include "table/ItemConfigMgr.h"

INT32 CMSIdipQueryItemIdToNameHandler::HandlerRequest(const rapidjson::Value& roHeadJson, const rapidjson::Value& roBodyJson, CIdipReply& roReply)
{
	INT32 itemid = JsonUtil::GetJsonInt(roBodyJson, "ItemId");
	std::string strItemName = JsonUtil::GetJsonString(roBodyJson, "ItemName");
	if (itemid <= 0 && strItemName.empty())
	{
		roReply.m_nErrCode = IDIP_ERR_INVALID_PARA;
		roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
		return IDIP_SERVER_NONE;
	}
	if (itemid)
	{
		strItemName = CItemConfig::Instance()->GetItemName(itemid);
	}
	else if (!strItemName.empty())
	{
		itemid = CItemConfig::Instance()->GetItemID(strItemName);
	}

	if (itemid <= 0 || strItemName.empty())
	{
		roReply.m_nErrCode = IDIP_ERR_INVALID_PARA;
		roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
		return IDIP_SERVER_NONE;
	}
	JsonBuilder oBuilder;
	roReply.m_nErrCode = IDIP_ERR_SUCCESS;
	oBuilder.AddMember_Int("Result", roReply.m_nErrCode);
	oBuilder.AddMember_String("RetMsg", GetIdipErrDesc(roReply.m_nErrCode));
	oBuilder.AddMember_Int("ItemId", itemid);
	oBuilder.AddMember_String("ItemName", UrlEncode(strItemName));
	roReply.m_strResult = oBuilder.GetString();
	return IDIP_SERVER_NONE;
}
