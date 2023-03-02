#include "pch.h"
#include "commondef_handler.h"
#include "util/jsonutil.h"
#include "mail/mailop.h"
#include "define/maildef.h"
#include "gamelogic/rolesummarymgr.h"
#include "table/globalconfig.h"
#include "gamelogic/bagtransition.h"
#include "unit/rolemanager.h"
#include "utility/tlogger.h"

bool CIdipUpdateFatigueHandler::Process(const rapidjson::Value& roHeadJson, const rapidjson::Value& roBodyJson, CIdipReply& roReply)
{
	std::string strOpenId = JsonUtil::GetJsonString(roBodyJson, "OpenId");
	UINT64 qwRoleID = JsonUtil::GetJsonUint64(roBodyJson, "RoleId");
	INT32 Number	= JsonUtil::GetJsonInt(roBodyJson, "Num");
	if(Number >= 0)
	{
		roReply.m_nErrCode = IDIP_ERR_INVALID_REQ_NUM;
		roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
		return true;
	}

	INT32 nNumber = Number>0?Number:-1*Number;
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
	if (nNumber > pBag->CountVirtualItem(FATIGUE))
	{
		nNumber = pBag->CountVirtualItem(FATIGUE);
	}
	BagTakeItemTransition take(pRole);
	take.SetReason(ItemFlow_IDIP, ItemFlow_IDIP_Oper);
	if(!take.TakeItem(FATIGUE, nNumber))
	{
		take.RollBack();
		roReply.m_nErrCode = IDIP_ERR_NUM_NOT_ENOUGH;
		roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
		return true;
	}
	take.NotifyClient();
	roReply.m_nErrCode = IDIP_ERR_SUCCESS;
	roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
	return true;
}
