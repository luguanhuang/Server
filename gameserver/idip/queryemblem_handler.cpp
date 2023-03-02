#include "pch.h"
#include "commondef_handler.h"
#include "idip/idiperrdesc.h"
#include "util/jsonutil.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "gamelogic/bag.h"
#include "gamelogic/itemconfig.h"
#include "define/itemdef.h"

bool BuildEmblemInfo(std::vector<XItem*> items, rapidjson::Value& oItemList, JsonBuilder& oBuilder, INT32 pos)
{
	for (auto i = items.begin(); i != items.end(); ++i)
	{
		rapidjson::Value item(rapidjson::kObjectType);
		item.AddMember("Position", pos, oBuilder.Allocator());
		item.AddMember("BadgeId", (*i)->itemID, oBuilder.Allocator());
		item.AddMember("BadgeName", UrlEncode(ItemConfig::Instance()->GetItemName((*i)->itemID)), oBuilder.Allocator());
		item.AddMember("BadgeUId", ToString((*i)->uid), oBuilder.Allocator());
		for(INT32 j = 0; j < MAX_BADGEATTRNUM; j ++)
		{
			ItemChangeAttr attr;
			if (j < (*i)->changeAttr.size())
			{
				attr = (*i)->changeAttr[j];
			}
			INT32 index = j + 1;
			std::string strAttrID = "BadgeAttr" + ToString(index);
			std::string strAttrValue = "AttrNum" + ToString(index);
			rapidjson::Value info(rapidjson::kStringType);
			info.SetString(strAttrID.c_str(), oBuilder.Allocator());
			item.AddMember(info, attr.AttrID, oBuilder.Allocator());
			info.SetString(strAttrValue.c_str(), oBuilder.Allocator());
			item.AddMember(info, attr.AttrValue, oBuilder.Allocator());
		}
		oItemList.PushBack(item, oBuilder.Allocator());
	}
	return true;
}
bool CIdipQueryEmblemHandler::Process(const rapidjson::Value& roHeadJson, const rapidjson::Value& roBodyJson, CIdipReply& roReply)
{
	std::string strOpenId = JsonUtil::GetJsonString(roBodyJson, "OpenId");
	UINT64 uRoleId = JsonUtil::GetJsonUint64(roBodyJson, "RoleId");

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
	UINT32 uItemCount = 0;
	bag->GetBodyEmblem(items);
	BuildEmblemInfo(items, oItemList, oBuilder, 0);
	items.clear();
	bag->GetBagItem(items, EMBLEM);
	BuildEmblemInfo(items, oItemList, oBuilder, 1);
	uItemCount += oItemList.Size();
	oBuilder.AddMember_Uint("BadgeList_count", uItemCount);
	oBuilder.AddMember_Value("BadgeList", oItemList);

	roReply.m_nErrCode = IDIP_ERR_SUCCESS;
	roReply.m_strResult = oBuilder.GetString();
	return true;
}
