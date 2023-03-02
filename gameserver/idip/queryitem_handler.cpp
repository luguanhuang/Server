#include "pch.h"
#include "commondef_handler.h"
#include "idip/idiperrdesc.h"
#include "util/jsonutil.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "gamelogic/bag.h"
#include "gamelogic/itemconfig.h"
#include "define/itemdef.h"

bool BuildItemInfo(XItem* pItem, rapidjson::Value& oItemList, JsonBuilder& oBuilder, INT32 pos)
{
		rapidjson::Value item(rapidjson::kObjectType);
		item.AddMember("Position", pos, oBuilder.Allocator());
		item.AddMember("EquipItemId", pItem->itemID, oBuilder.Allocator());
		item.AddMember("EquipItemName", UrlEncode(ItemConfig::Instance()->GetItemName(pItem->itemID)), oBuilder.Allocator());
		item.AddMember("EquipItemLevel", pItem->GetEnhanceLevel(), oBuilder.Allocator());
		for (INT32 j = 0; j < MAX_DRAGONJADENUM; j ++)
		{
			INT32 jadeid = 0;
			std::string jadename("null");
			if (NULL != pItem->jadeInfo)
			{
				JadeSingle* pJadeSingle = pItem->jadeInfo->GetJadeSingle(j);
				if(NULL != pJadeSingle)
				{
					jadeid = pJadeSingle->jadeid;
					jadename = UrlEncode(ItemConfig::Instance()->GetItemName(pJadeSingle->jadeid));
				}
			}
			INT32 index = j + 1;
			std::string strJadeId = "DragonJadeId" + ToString(index);
			std::string strJadeName = "DragonJadeName" + ToString(index);
			rapidjson::Value info(rapidjson::kStringType);
			info.SetString(strJadeId.c_str(), oBuilder.Allocator());
			item.AddMember(info, jadeid, oBuilder.Allocator());
			info.SetString(strJadeName.c_str(), oBuilder.Allocator());
			item.AddMember(info, jadename, oBuilder.Allocator());
		}
		item.AddMember("EquipItemUId", ToString(pItem->uid), oBuilder.Allocator());


		std::vector<ItemChangeAttr> vList;
		for (INT32 j = 0; j < MAX_SMELTATTRNUM; j ++)
		{
			ItemChangeAttr attr;
			if (NULL != pItem->randAttr)
			{
				if (j < pItem->randAttr->attrs.size())
				{
					attr.AttrID = pItem->randAttr->attrs[j].AttrID;
					attr.AttrValue = pItem->randAttr->attrs[j].AttrValue;
				}
			}
			vList.push_back(attr);
		}

		for (INT32 j = 0; j < MAX_REFINEATTRNUM; j ++)
		{
			ItemChangeAttr attr;
			if (NULL != pItem->forge)
			{
				if (j < pItem->forge->attrs.size())
				{
					attr= pItem->forge->attrs[j];
				}
			}
			vList.push_back(attr);
		}

		for (UINT32 j = 0; j < vList.size(); j ++)
		{
			INT32 index = j + 1;
			std::string strAttrID = "RefineAttr" + ToString(index);
			std::string strAttrValue = "AttrValue" + ToString(index);
			rapidjson::Value info(rapidjson::kStringType);
			info.SetString(strAttrID.c_str(), oBuilder.Allocator());
			item.AddMember(info, vList[j].AttrID, oBuilder.Allocator());
			info.SetString(strAttrValue.c_str(), oBuilder.Allocator());
			item.AddMember(info, vList[j].AttrValue, oBuilder.Allocator());
		}

		ItemChangeAttr attr1;
		if (NULL != pItem->enchant)
		{
			attr1.AttrID = pItem->enchant->chooseAttrid;
			auto it = pItem->enchant->mapAllAttrs.find(pItem->enchant->chooseAttrid);
			if (it != pItem->enchant->mapAllAttrs.end())
			{
				attr1.AttrValue = it->second.attrValue;
			}
		}
		item.AddMember("SoulAttr", attr1.AttrID, oBuilder.Allocator());
		item.AddMember("SoulAttrValue", attr1.AttrValue, oBuilder.Allocator());
		oItemList.PushBack(item, oBuilder.Allocator());
	return true;
}
bool CIdipQueryItemHandler::Process(const rapidjson::Value& roHeadJson, const rapidjson::Value& roBodyJson, CIdipReply& roReply)
{
	std::string strOpenId = JsonUtil::GetJsonString(roBodyJson, "OpenId");
	UINT64 uRoleId = JsonUtil::GetJsonUint64(roBodyJson, "RoleId");
	INT32 PageNo = JsonUtil::GetJsonInt(roBodyJson, "PageNo");

	Role* pRole = RoleManager::Instance()->FindByRoleIDOrNickID(uRoleId);
	if (NULL == pRole)
	{
		roReply.m_nErrCode = IDIP_ERR_PLAYER_NOT_EXIST;
		roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
		return true;
	}

	Bag* bag = pRole->Get<Bag>();
	if(NULL == bag)
	{
		roReply.m_nErrCode = IDIP_ERR_UNKNOWN;
		roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
		return true;
	}
	JsonBuilder oBuilder;
	rapidjson::Value oItemList(rapidjson::kArrayType);
	std::vector<XItem*> items;
	std::map<XItem*, INT32> vitems;
	bag->GetBodyEquip(items);
	for (UINT32 i = 0; i < items.size(); i ++)
	{
		vitems[items[i]] = 0;
	}
	items.clear();
	bag->GetBagItem(items, EQUIP);
	for (UINT32 i = 0; i < items.size(); i ++)
	{
		vitems[items[i]] = 1;
	}

	INT32 iStart = 0;
	INT32 iEnd = 0;
	if (!GetPageNo(vitems.size(), PageNo, MAX_EQUIPITEMLIST_NUM, iStart, iEnd))
	{
		roReply.m_nErrCode = IDIP_ERR_INVALID_PARA;
		roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
		return true;
	}
	INT32 i = 0;
	for (auto it = vitems.begin(); it != vitems.end(); it ++)
	{
		if (i >= iStart && i < iEnd)
		{
			BuildItemInfo(it->first, oItemList, oBuilder, it->second);
		}
		i ++;
	}
	oBuilder.AddMember_Uint("EquipItemList_count", oItemList.Size());
	oBuilder.AddMember_Value("EquipItemList", oItemList);
	INT32 TotalPageNo = (vitems.size() + MAX_EQUIPITEMLIST_NUM -1) / MAX_EQUIPITEMLIST_NUM;
	oBuilder.AddMember_Int("TotalPageNo", TotalPageNo);
	
	roReply.m_nErrCode = IDIP_ERR_SUCCESS;
	roReply.m_strResult = oBuilder.GetString();
	return true;
}
