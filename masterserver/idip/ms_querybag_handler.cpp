#include "pch.h"
#include "ms_commondef_handler.h"
#include "util/jsonutil.h"
#include "role/bag.h"
#include "table/ItemConfigMgr.h"

IDIP_HANDLER_READ_IMPLEMETION(QueryBag)
INT32 CMSIdipQueryBagHandler::HandlerRequest(const rapidjson::Value& roHeadJson, const rapidjson::Value& roBodyJson, CIdipReply& roReply)
{
	UINT64 roleid = JsonUtil::GetJsonUint64(roBodyJson, "RoleId");
	szOpenId = JsonUtil::GetJsonString(roBodyJson, "OpenId");
	if (!CRoleSummaryMgr::Instance()->IsValidAcountRole(szOpenId, roleid))
	{
		roReply.m_nErrCode = IDIP_ERR_PLAYER_NOT_EXIST;
		roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
		return IDIP_SERVER_NONE;
	}
	m_id1 = JsonUtil::GetJsonInt(roBodyJson, "PageNo");
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
bool CMSIdipQueryBagHandler::OnAsyncReply(const CUserData& roUserData)
{
	if(roUserData.m_pUserPtr == NULL)
	{
		//timeout
		CRpc::ReplyDelayRpc(GetRpcID(), roUserData);
	}
	else
	{		
		std::string str;
		CIdipReply oReply(str);
		CUserData data;
		data.m_pUserPtr = &oReply;
		if(roUserData.m_dwUserData != KKSG::ERR_SUCCESS)
		{
			oReply.m_nErrCode = IDIP_ERR_UNKNOWN;
			oReply.m_strResult = PackErrResponse(oReply.m_nErrCode);
			CRpc::ReplyDelayRpc(GetRpcID(), data);
			return true;
		}
		RoleAllInfo* pInfo = (RoleAllInfo*)roUserData.m_pUserPtr;
		const KKSG::BagContent &roBag = pInfo->bag();
		JsonBuilder oBuilder;
		rapidjson::Value oItemList(rapidjson::kArrayType);
		UINT32 uItemCount = 0;
		std::vector<Item> items;
		//背包里的装备
		for (int i = 0; i < roBag.items_size(); ++i)
		{
			KKSG::Item rItem = roBag.items(i);
			//过滤装备和纹章信息
			if (rItem.itemtype() == EQUIP || rItem.itemtype() == EMBLEM || rItem.itemid() == 0)
			{
				continue;
			}
			items.push_back(rItem);
		}

		//身上的龙器；
		for (int i = 0; i < roBag.artifacts_size(); ++i)
		{
			if (roBag.artifacts(i).itemid())
			{
				items.push_back(roBag.artifacts(i));
			}
		}

		INT32 iStart = (m_id1 - 1) * MAX_BAGLIST_NUM;
		if (iStart < 0 || (items.size() && iStart >= (INT32)items.size()))
		{
			oReply.m_nErrCode = IDIP_ERR_INVALID_PARA;
			oReply.m_strResult = PackErrResponse(oReply.m_nErrCode);
			CRpc::ReplyDelayRpc(GetRpcID(), data);
			return true;
		}
		UINT32 uTotalPageNo = (items.size() + MAX_BAGLIST_NUM -1) / MAX_BAGLIST_NUM;
		UINT32 uBanInfoListCount = 0;

		INT32 iEnd = items.size() < (m_id1 * MAX_BAGLIST_NUM) ? items.size() : m_id1 * MAX_BAGLIST_NUM;
		for (INT32 i = iStart; i < iEnd; ++ i)
		{
			rapidjson::Value item(rapidjson::kObjectType);
			item.AddMember("ItemId", items[i].itemid(), oBuilder.Allocator());
			item.AddMember("ItemName", UrlEncode(CItemConfig::Instance()->GetItemName(items[i].itemid())), oBuilder.Allocator());
			item.AddMember("ItemNum", items[i].itemcount(), oBuilder.Allocator());
			oItemList.PushBack(item, oBuilder.Allocator());
			uItemCount ++;
		}

		oBuilder.AddMember_Uint("BagList_count", uItemCount);
		oBuilder.AddMember_Value("BagList", oItemList);
		oBuilder.AddMember_Uint("TotalPageNo", uTotalPageNo);

		oReply.m_nErrCode = IDIP_ERR_SUCCESS;
		oReply.m_strResult = oBuilder.GetString();
		CRpc::ReplyDelayRpc(GetRpcID(), data);
	}
	return true;
}
