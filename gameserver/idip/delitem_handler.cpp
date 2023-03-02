#include "pch.h"
#include "commondef_handler.h"
#include "util/jsonutil.h"
#include "gamelogic/rolesummarymgr.h"
#include "gamelogic/itemconfig.h"
#include "unit/rolemanager.h"
#include "gamelogic/bag.h"
#include "gamelogic/bagtransition.h"
#include "utility/tlogger.h"


bool CIdipDelItemHandler::Process(const rapidjson::Value& roHeadJson, const rapidjson::Value& roBodyJson, CIdipReply& roReply)
{
	std::string strOpenId = JsonUtil::GetJsonString(roBodyJson, "OpenId");
	UINT64 qwRoleID = JsonUtil::GetJsonUint64(roBodyJson, "RoleId");
	INT32 nItemID	= JsonUtil::GetJsonInt(roBodyJson, "ItemId");
	INT32 nCount	= JsonUtil::GetJsonInt(roBodyJson, "ItemNum");
	INT32 Count = nCount;
	if(nCount <= 0)
	{
		roReply.m_nErrCode = IDIP_ERR_INVALID_REQ_NUM;
		roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
		return true;
	}
	if(ItemConfig::Instance()->GetItemConf(nItemID) == NULL)
	{
		roReply.m_nErrCode = IDIP_ERR_INVALID_ITEM_ID;
		roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
		return true;
	}

	Role* pRole = RoleManager::Instance()->FindByRoleIDOrNickID(qwRoleID);
	if (NULL == pRole)
	{
		roReply.m_nErrCode = IDIP_ERR_PLAYER_NOT_EXIST;
		roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
		return true;
	}

	Bag* pBag = pRole->Get<Bag>();
	if (NULL == pBag)
	{
		roReply.m_nErrCode = IDIP_ERR_UNKNOWN;
		roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
		return true;
	}
	INT32 count = (INT32)pBag->CountItemInBag(nItemID);
	if (nCount > count)
	{
		nCount = count;
	}
	BagTakeItemTransition take(pRole);
	take.SetReason(ItemFlow_IDIP, ItemFlow_IDIP_Oper);
	if(!take.TakeItem((UINT32)nItemID, nCount))
	{
		take.RollBack();
		roReply.m_nErrCode = IDIP_ERR_UNKNOWN;
		roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
		return true;
	}
	take.NotifyClient();
	roReply.m_nErrCode = IDIP_ERR_SUCCESS;
	roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
	return true;
}

