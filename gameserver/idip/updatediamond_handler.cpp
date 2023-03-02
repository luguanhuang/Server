#include "pch.h"
#include "commondef_handler.h"
#include "util/jsonutil.h"
#include "mail/mailop.h"
#include "define/maildef.h"
#include "gamelogic/rolesummarymgr.h"
#include "gamelogic/itemconfig.h"
#include "table/globalconfig.h"
#include "gamelogic/bagtransition.h"
#include "unit/rolemanager.h"
#include "utility/tlogger.h"


bool CIdipUpdateDiamondHandler::Process(const rapidjson::Value& roHeadJson, const rapidjson::Value& roBodyJson, CIdipReply& roReply)
{
	std::string strOpenId = JsonUtil::GetJsonString(roBodyJson, "OpenId");
	UINT64 qwRoleID = JsonUtil::GetJsonUint64(roBodyJson, "RoleId");
	INT64 Number = 0;
	if (GetCmd() != IDIP_AQ_DO_UPDATE_DIAMOND_REQ)
	{
		Number	= JsonUtil::GetJsonInt64(roBodyJson, "DiamondNum");
	}
	else
	{
		Number	= JsonUtil::GetJsonInt(roBodyJson, "Num");
	}
	if(Number >= 0)
	{
		roReply.m_nErrCode = IDIP_ERR_INVALID_REQ_NUM;
		roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
		return true;
	}

	INT64 nNumber = Number>0?Number:-1*Number;
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
	if (nNumber > pBag->CountVirtualItem(DIAMOND))
	{
		nNumber = pBag->CountVirtualItem(DIAMOND);
	}

	KKSG::PayConsumeBase info;
	info.set_amt(nNumber);
	info.set_subreason(ItemFlow_IDIP_Oper);
	info.set_reason(ItemFlow_IDIP);
	std::vector<ItemDesc> vecItem;
	if(!pRole->ConsumeDiamond(info,vecItem))
	{
		roReply.m_nErrCode = IDIP_ERR_NUM_NOT_ENOUGH;
		roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
		return true;
	}
	roReply.m_nErrCode = IDIP_ERR_SUCCESS;
	roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
	return true;
}

