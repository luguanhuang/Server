#include "pch.h"
#include "ms_commondef_handler.h"
#include "util/jsonutil.h"
#include "table/ItemConfigMgr.h"
#include "util/XCommon.h"

INT32 CMSIdipSendItemHandler::HandlerRequest(const rapidjson::Value& roHeadJson, const rapidjson::Value& roBodyJson, CIdipReply& roReply)
{
	roReply.m_nErrCode = IDIP_ERR_SUCCESS;
	std::string strOpenId = JsonUtil::GetJsonString(roBodyJson, "OpenId");
	INT32 nItemType = JsonUtil::GetJsonInt(roBodyJson, "ItemType");
	std::string	strMailTitle	= JsonUtil::GetJsonString(roBodyJson, "MailTitle");
	std::string	strMailContent	= JsonUtil::GetJsonString(roBodyJson, "MailContent");
	INT32 iDays = JsonUtil::GetJsonInt(roBodyJson, "Time");
	UINT32 uBind = JsonUtil::GetJsonUint(roBodyJson, "IsBind");

	std::vector<ItemDesc> items;
	if (roBodyJson.HasMember("ItemList_count") && roBodyJson.HasMember("ItemList") && 
		roBodyJson.HasMember("IsBind"))
	{
		bool bFlag = true;
		if (uBind == 0)
		{
			bFlag = false;
		}
		UINT32 uItemListCount = JsonUtil::GetJsonUint(roBodyJson, "ItemList_count");
		const rapidjson::Value& roMember = JsonUtil::GetJsonValue(roBodyJson, "ItemList");
		if (roMember.IsNull())
		{
			roReply.m_nErrCode = IDIP_ERR_INVALID_PARA;
			roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
			return IDIP_SERVER_NONE;
		}
		if (roMember.IsArray())
		{
			for (UINT32 i = 0; i < roMember.Size(); i ++)
			{
				UINT64 uItemId = JsonUtil::GetJsonUint64(roMember[i], "ItemId");
				UINT32 uItemNum = JsonUtil::GetJsonUint(roMember[i], "ItemNum");
				if(CItemConfig::Instance()->GetItemConf(uItemId) == NULL)
				{
					roReply.m_nErrCode = IDIP_ERR_INVALID_ITEM_ID;
					roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
					return IDIP_SERVER_NONE;
				}
				if(uItemNum <= 0)
				{
					roReply.m_nErrCode = IDIP_ERR_INVALID_REQ_NUM;
					roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
					return IDIP_SERVER_NONE;
				}
				ItemDesc pdesc(uItemId, uItemNum, bFlag);
				items.push_back(pdesc);
			}
		}
	}
	else if (roBodyJson.HasMember("ItemId") && roBodyJson.HasMember("ItemNum"))
	{
		INT32 nItemID	= JsonUtil::GetJsonInt(roBodyJson, "ItemId");
		INT32 nCount	= JsonUtil::GetJsonInt(roBodyJson, "ItemNum");
		if(nCount <= 0)
		{
			roReply.m_nErrCode = IDIP_ERR_INVALID_REQ_NUM;
			roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
			return IDIP_SERVER_NONE;
		}
		if(CItemConfig::Instance()->GetItemConf(nItemID) == NULL)
		{
			roReply.m_nErrCode = IDIP_ERR_INVALID_ITEM_ID;
			roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
			return IDIP_SERVER_NONE;
		}
		ItemDesc pdesc(nItemID, nCount);
		items.push_back(pdesc);
	}
	else
	{
		roReply.m_nErrCode = IDIP_ERR_INVALID_PARA;
		roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
		return IDIP_SERVER_NONE;
	}
	if (iDays < 0)
	{
		roReply.m_nErrCode = IDIP_ERR_INVALID_TIME;
		roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
		return IDIP_SERVER_NONE;
	}

	UINT32 uExpiretime = !iDays?FOREVER_TIME:XCommon::GetOneDayTime() * iDays;
	MailData oData(strMailTitle, strMailContent, items, uExpiretime);
	oData.SetReason(ItemFlow_IDIP_Oper);
	if (GetCmd() == IDIP_DO_GM_SEND_ITEM_REQ)
	{
		if (!CMailMgr::Instance()->SendSystemMail(oData))
		{
			roReply.m_nErrCode = IDIP_ERR_SEND_MAIL;
		}
		else
		{
			for (UINT32 i = 0; i < items.size(); i ++)
			{
				TIdipDoGmSendItemLog oLog("IdipDoGmSendItemFlow");
				oLog.m_uItemID = items[i].itemID;
				oLog.m_nNum = items[i].itemCount;
				oLog.m_uBind = uBind;
				oLog.EndTime = iDays;
				oLog.Do(roHeadJson, roBodyJson);
			}
		}
		roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
		return IDIP_SERVER_NONE;
	}

	UINT64 qwRoleID = JsonUtil::GetJsonUint64(roBodyJson, "RoleId");
	if (!CRoleSummaryMgr::Instance()->IsValidAcountRole(strOpenId, qwRoleID))
	{
		roReply.m_nErrCode = IDIP_ERR_PLAYER_NOT_EXIST;
		roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
		return IDIP_SERVER_NONE;
	}
	if (!CMailMgr::Instance()->SendMail(qwRoleID, oData))
	{
		roReply.m_nErrCode = IDIP_ERR_SEND_MAIL;
	}
	else
	{
		for (UINT32 i = 0; i < items.size(); i ++)
		{
			TIdipSendItemLog oLog("IdipSendItemFlow");
			oLog.m_uItemID = items[i].itemID;
			oLog.m_nNum = items[i].itemCount;
			oLog.m_uBind = uBind;
			oLog.m_uRoleId = qwRoleID;
			oLog.szOpenId = strOpenId;
			oLog.EndTime = iDays; 
			oLog.Do(roHeadJson, roBodyJson);
		}
	}
	roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
	return IDIP_SERVER_NONE;
}
