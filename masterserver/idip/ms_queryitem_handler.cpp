#include "pch.h"
#include "ms_commondef_handler.h"
#include "util/jsonutil.h"
#include "role/bag.h"
#include "table/ItemConfigMgr.h"

IDIP_HANDLER_READ_IMPLEMETION(QueryItemInfo)
INT32 CMSIdipQueryItemInfoHandler::HandlerRequest(const rapidjson::Value& roHeadJson, const rapidjson::Value& roBodyJson, CIdipReply& roReply)
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
	m_id1 = JsonUtil::GetJsonInt(roBodyJson, "PageNo");
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
bool Build(std::vector<KKSG::Item>& items, rapidjson::Value& oItemList, JsonBuilder& oBuilder, INT32 start, INT32 end)
{
	for (auto i = start; i < end; i ++)
	{
		const KKSG::Item& rItem = items[i];
		rapidjson::Value item(rapidjson::kObjectType);
		item.AddMember("Position", rItem.itemtype(), oBuilder.Allocator());
		item.AddMember("EquipItemId", rItem.itemid(), oBuilder.Allocator());
		item.AddMember("EquipItemName", UrlEncode(CItemConfig::Instance()->GetItemName(rItem.itemid())), oBuilder.Allocator());
		UINT32 uLevel = 0;
		if (rItem.has_enhancelevel())
		{
			uLevel = rItem.enhancelevel();
		}
		item.AddMember("EquipItemLevel", uLevel, oBuilder.Allocator());

		INT32 jadeid1 = 0;
		std::string jadename1("null");
		INT32 jadeid2 = 0;
		std::string jadename2("null");
		INT32 jadeid3 = 0;
		std::string jadename3("null");
		INT32 jadeid4 = 0;
		std::string jadename4("null");
		if (rItem.has_itemjade())
		{
			for (int j = 0; j < rItem.itemjade().itemjadesingle_size(); ++ j)
			{
				if (rItem.itemjade().itemjadesingle(j).slotpos() == 0)
				{
					jadeid1 = rItem.itemjade().itemjadesingle(j).itemid();
					jadename1 = UrlEncode(CItemConfig::Instance()->GetItemName(jadeid1));
				}
				else if (rItem.itemjade().itemjadesingle(j).slotpos() == 1)
				{
					jadeid2 = rItem.itemjade().itemjadesingle(j).itemid();
					jadename2 = UrlEncode(CItemConfig::Instance()->GetItemName(jadeid2));
				}

				else if (rItem.itemjade().itemjadesingle(j).slotpos() == 2)
				{
					jadeid3 = rItem.itemjade().itemjadesingle(j).itemid();
					jadename3 = UrlEncode(CItemConfig::Instance()->GetItemName(jadeid3));
				}
				else if (rItem.itemjade().itemjadesingle(j).slotpos() == 3)
				{
					jadeid4 = rItem.itemjade().itemjadesingle(j).itemid();
					jadename4 = UrlEncode(CItemConfig::Instance()->GetItemName(jadeid4));
				}
			}
			item.AddMember("DragonJadeId1", jadeid1, oBuilder.Allocator());
			item.AddMember("DragonJadeName1", jadename1, oBuilder.Allocator());
			item.AddMember("DragonJadeId2", jadeid2, oBuilder.Allocator());
			item.AddMember("DragonJadeName2", jadename2, oBuilder.Allocator());
			item.AddMember("DragonJadeId3", jadeid3, oBuilder.Allocator());
			item.AddMember("DragonJadeName3", jadename3, oBuilder.Allocator());
			item.AddMember("DragonJadeId4", jadeid4, oBuilder.Allocator());
			item.AddMember("DragonJadeName4", jadename4, oBuilder.Allocator());
			item.AddMember("EquipItemUId", ToString(rItem.uid()), oBuilder.Allocator());

			std::vector<std::pair<UINT32, UINT32> > vList;
			for (INT32 j = 0; j < MAX_SMELTATTRNUM; j ++)
			{
				std::pair<UINT32, UINT32> attr = std::make_pair(0 ,0);
				if (rItem.has_randattr())
				{
					if (j < rItem.randattr().attrs_size())
					{
						attr.first = rItem.randattr().attrs(j).id();
						attr.second = rItem.randattr().attrs(j).value();
					}
				}
				vList.push_back(attr);
			}

			for (INT32 j = 0; j < MAX_REFINEATTRNUM; j ++)
			{
				UINT32 id = 0;
				UINT32 value = 0;
				if (rItem.has_forge())
				{
					if (j < rItem.forge().attrs_size())
					{
						id= rItem.forge().attrs(j).id();
						value = rItem.forge().attrs(j).value();
					}
				}
				vList.push_back(std::make_pair(id, value));
			}

			for (UINT32 j = 0; j < vList.size(); j ++)
			{
				INT32 index = j + 1;
				std::string strAttrID = "RefineAttr" + ToString(index);
				std::string strAttrValue = "AttrValue" + ToString(index);
				rapidjson::Value info(rapidjson::kStringType);
				info.SetString(strAttrID.c_str(), oBuilder.Allocator());
				item.AddMember(info, vList[j].first, oBuilder.Allocator());
				info.SetString(strAttrValue.c_str(), oBuilder.Allocator());
				item.AddMember(info, vList[j].second, oBuilder.Allocator());
			}

			UINT32 id = 0;
			UINT32 value = 0;
			if (rItem.has_enchant())
			{
				id = rItem.enchant().chooseattrid();
				for (int j = 0; j < rItem.enchant().allattrs_size(); j++)
				{
					if (rItem.enchant().allattrs(j).id() == id)
					{
						value = rItem.enchant().allattrs(j).value();
						break;
					}
				}
			}
			item.AddMember("SoulAttr", id, oBuilder.Allocator());
			item.AddMember("SoulAttrValue", value, oBuilder.Allocator());
			oItemList.PushBack(item, oBuilder.Allocator());
		}
	}
	return true;
}
bool CMSIdipQueryItemInfoHandler::OnAsyncReply(const CUserData& roUserData)
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
		KKSG::Item rItem;
		std::vector<KKSG::Item> items;
		//背包里的装备
		for (int i = 0; i < roBag.items_size(); ++i)
		{
			rItem = roBag.items(i);
			//过滤装备和纹章信息
			if (rItem.itemtype() != EQUIP || rItem.itemid() == 0)
			{
				continue;
			}
			rItem.set_itemtype(1);
			items.push_back(rItem);
		}
		//身上的
		for (int i = 0; i < roBag.equips_size(); ++i)
		{
			rItem = roBag.equips(i);
			if (rItem.itemid() == 0)
			{
				continue;
			}
			rItem.set_itemtype(0);
			items.push_back(rItem);
		}
		INT32 iStart = 0;
		INT32 iEnd = 0;
		if (!GetPageNo(items.size(), m_id1, MAX_EQUIPITEMLIST_NUM, iStart, iEnd))
		{
			oReply.m_nErrCode = IDIP_ERR_INVALID_PARA;
			oReply.m_strResult = PackErrResponse(oReply.m_nErrCode);
			CRpc::ReplyDelayRpc(GetRpcID(), data);
			return true;
		}
		Build(items, oItemList, oBuilder, iStart, iEnd);
		oBuilder.AddMember_Uint("EquipItemList_count", oItemList.Size());
		oBuilder.AddMember_Value("EquipItemList", oItemList);
		INT32 TotalPageNo = (items.size() + MAX_EQUIPITEMLIST_NUM -1) / MAX_EQUIPITEMLIST_NUM;
		oBuilder.AddMember_Int("TotalPageNo", TotalPageNo);
		oReply.m_nErrCode = IDIP_ERR_SUCCESS;
		oReply.m_strResult = oBuilder.GetString();
		CRpc::ReplyDelayRpc(GetRpcID(), data);
	}
	return true;
}
