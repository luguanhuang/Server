#include "pch.h"
#include "ms_commondef_handler.h"
#include "util/jsonutil.h"
#include "role/bag.h"
#include "table/ItemConfigMgr.h"
#include "idip/idip.h"

static bool BuildBadgeInfo(const Item& rItem, JsonBuilder& oBuilder, rapidjson::Value& oItemList, INT32 pos)
{
	if (rItem.itemtype() != EMBLEM || rItem.itemid() == 0)
	{
		return false;
	}
	rapidjson::Value item(rapidjson::kObjectType);
	item.AddMember("Position", pos, oBuilder.Allocator());
	item.AddMember("BadgeId", rItem.itemid(), oBuilder.Allocator());
	item.AddMember("BadgeName", UrlEncode(CItemConfig::Instance()->GetItemName(rItem.itemid())), oBuilder.Allocator());
	item.AddMember("BadgeUId", ToString(rItem.uid()), oBuilder.Allocator());
	for(INT32 j = 0; j < MAX_BADGEATTRNUM; j ++)
	{
		UINT32 AttrID = 0;
		UINT32 AttrValue = 0;
		if (j < rItem.attrid_size() && j < rItem.attrvalue_size())
		{
			AttrID = rItem.attrid(j);
			AttrValue = rItem.attrvalue(j);
		}
		INT32 index = j + 1;
		std::string strAttrID = "BadgeAttr" + ToString(index);
		std::string strAttrValue = "AttrNum" + ToString(index);
		rapidjson::Value info(rapidjson::kStringType);
		info.SetString(strAttrID.c_str(), oBuilder.Allocator());
		item.AddMember(info, AttrID, oBuilder.Allocator());
		info.SetString(strAttrValue.c_str(), oBuilder.Allocator());
		item.AddMember(info, AttrValue, oBuilder.Allocator());
	}
	oItemList.PushBack(item, oBuilder.Allocator());
	return true;
}

IDIP_HANDLER_READ_IMPLEMETION(QueryEmblem)
INT32 CMSIdipQueryEmblemHandler::HandlerRequest(const rapidjson::Value& roHeadJson, const rapidjson::Value& roBodyJson, CIdipReply& roReply)
{
	UINT64 roleid = JsonUtil::GetJsonUint64(roBodyJson, "RoleId");
	szOpenId = JsonUtil::GetJsonString(roBodyJson, "OpenId");
	if (!CRoleSummaryMgr::Instance()->IsValidAcountRole(szOpenId, roleid))
	{
		roReply.m_nErrCode = IDIP_ERR_PLAYER_NOT_EXIST;
		roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
		return IDIP_SERVER_NONE;
	}
	SetRoleID(roleid);

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
bool CMSIdipQueryEmblemHandler::OnAsyncReply(const CUserData& roUserData)
{
	if(roUserData.m_pUserPtr == NULL)
	{
		//timeout
		CRpc::ReplyDelayRpc(GetRpcID(), roUserData);
	}
	else
	{		
		if(roUserData.m_dwUserData != KKSG::ERR_SUCCESS)
		{
			std::string str;
			CIdipReply oReply(str);
			oReply.m_nErrCode = IDIP_ERR_UNKNOWN;
			oReply.m_strResult = PackErrResponse(oReply.m_nErrCode);
			CUserData data;
			data.m_pUserPtr = &oReply;
			CRpc::ReplyDelayRpc(GetRpcID(), data);
			return true;
		}

		RoleAllInfo* pInfo = (RoleAllInfo*)roUserData.m_pUserPtr;
		const KKSG::BagContent &roBag = pInfo->bag();
		JsonBuilder oBuilder;
		rapidjson::Value oItemList(rapidjson::kArrayType);
		UINT32 uItemCount = 0;
		KKSG::Item rItem;
		//背包里的装备
		for (int i = 0; i < roBag.items_size(); ++i)
		{
			rItem = roBag.items(i);
			if (BuildBadgeInfo(rItem, oBuilder, oItemList, 1))
			{
				uItemCount ++;
			}
		}

		//身上的
		for (int i = 0; i < roBag.emblems_size(); ++i)
		{
			rItem = roBag.emblems(i);
			if (BuildBadgeInfo(rItem, oBuilder, oItemList, 0))
			{
				uItemCount ++;
			}
		}
		oBuilder.AddMember_Uint("BadgeList_count", uItemCount);
		oBuilder.AddMember_Value("BadgeList", oItemList);

		std::string str = oBuilder.GetString();
		CIdipReply oReply(str);
		oReply.m_nErrCode = IDIP_ERR_SUCCESS;
		CUserData data;
		data.m_pUserPtr = &oReply;
		CRpc::ReplyDelayRpc(GetRpcID(), data);
	}
	return true;
}
