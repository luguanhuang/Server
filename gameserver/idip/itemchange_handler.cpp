#include "pch.h"
#include "commondef_handler.h"
#include "util/jsonutil.h"
#include "unit/rolemanager.h"
#include "gamelogic/bagtransition.h"

bool CIdipItemChangeHandler::Process(const rapidjson::Value& roHeadJson, const rapidjson::Value& roBodyJson, CIdipReply& roReply)
{
	std::string strOpenId = JsonUtil::GetJsonString(roBodyJson, "OpenId");
	UINT64 qwRoleID = JsonUtil::GetJsonUint64(roBodyJson, "RoleId");
	Role* pRole = RoleManager::Instance()->FindByRoleIDOrNickID(qwRoleID);
	if (NULL == pRole)
	{
		roReply.m_nErrCode = IDIP_ERR_PLAYER_NOT_EXIST;
		roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
		return true;
	}
	std::vector<ItemDesc> vSList;
	std::vector<ItemDesc> vDList;
	const rapidjson::Value& roMember = JsonUtil::GetJsonValue(roBodyJson, "SItemList");
	const rapidjson::Value& roDMember = JsonUtil::GetJsonValue(roBodyJson, "DItemList");

	if (!GetItemChangeList(roMember, vSList) || !GetItemChangeList(roDMember, vDList))
	{
		roReply.m_nErrCode = IDIP_ERR_INVALID_PARA;
		roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
		return true;
	}
	//道具id的合法性 ms已经验证过;（钻石已过滤掉）

	BagTakeItemTransition takeTransition(pRole);
	takeTransition.SetReason(ItemFlow_IDIP, ItemFlow_IdipExchange);

	for (auto iter = vDList.begin(); iter != vDList.end(); ++iter)
	{
		if(!takeTransition.TakeItem(iter->itemID, iter->itemCount))
		{
			takeTransition.RollBack();
			roReply.m_nErrCode = IDIP_ERR_NUM_NOT_ENOUGH;
			roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
			return true;
		}
	}

	takeTransition.NotifyClient();

	BagGiveItemTransition giveTransition(pRole);
	giveTransition.SetReason(ItemFlow_IDIP, ItemFlow_IdipExchange);

	for (auto it = vSList.begin(); it != vSList.end(); ++ it)
	{
		giveTransition.GiveItem(it->itemID, it->itemCount);
	}
	giveTransition.NotifyClient();

	roReply.m_nErrCode = IDIP_ERR_SUCCESS;
	roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
	return true;
}
