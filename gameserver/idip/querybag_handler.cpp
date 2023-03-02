#include "pch.h"
#include "commondef_handler.h"
#include "idip/idiperrdesc.h"
#include "util/jsonutil.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "gamelogic/bag.h"
#include "gamelogic/itemconfig.h"
#include "define/itemdef.h"

bool CIdipQueryBagHandler::Process(const rapidjson::Value& roHeadJson, const rapidjson::Value& roBodyJson, CIdipReply& roReply)
{
	std::string strOpenId = JsonUtil::GetJsonString(roBodyJson, "OpenId");
	UINT64 uRoleId = JsonUtil::GetJsonUint64(roBodyJson, "RoleId");
	UINT32 uPageNo = JsonUtil::GetJsonUint(roBodyJson, "PageNo");

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
	bag->GetBagItem(items);
	bag->GetBodyArtifact(items);
	for (auto it = items.begin(); it != items.end();)
	{
		if ((*it)->type == EQUIP || (*it)->type == EMBLEM || (*it)->itemID == 0)
		{
			it = items.erase(it);
		}
		else
		{
			it ++;
		}
	}
	INT32 iStart = (uPageNo - 1) * MAX_BAGLIST_NUM;
	if (iStart < 0 || (items.size() && iStart >= (INT32)items.size()))
	{
		roReply.m_nErrCode = IDIP_ERR_INVALID_PARA;
		roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
		return true;
	}
	UINT32 uTotalPageNo = (items.size() + MAX_BAGLIST_NUM -1) / MAX_BAGLIST_NUM;
	UINT32 uBanInfoListCount = 0;

	INT32 iEnd = items.size() < (uPageNo * MAX_BAGLIST_NUM) ? items.size() : uPageNo * MAX_BAGLIST_NUM;
	for (INT32 i = iStart; i < iEnd; ++ i)
	{
		rapidjson::Value item(rapidjson::kObjectType);
		item.AddMember("ItemId", items[i]->itemID, oBuilder.Allocator());
		item.AddMember("ItemName", UrlEncode(ItemConfig::Instance()->GetItemName(items[i]->itemID)), oBuilder.Allocator());
		item.AddMember("ItemNum", items[i]->itemCount, oBuilder.Allocator());
		oItemList.PushBack(item, oBuilder.Allocator());
		uItemCount ++;
	}
	oBuilder.AddMember_Uint("BagList_count", uItemCount);
	oBuilder.AddMember_Value("BagList", oItemList);
	oBuilder.AddMember_Uint("TotalPageNo", uTotalPageNo);

	roReply.m_nErrCode = IDIP_ERR_SUCCESS;
	roReply.m_strResult = oBuilder.GetString();
	return true;
}
