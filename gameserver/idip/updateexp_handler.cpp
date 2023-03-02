#include "pch.h"
#include "commondef_handler.h"
#include "util/jsonutil.h"
#include "mail/mailop.h"
#include "define/maildef.h"
#include "gamelogic/rolesummarymgr.h"
#include "table/globalconfig.h"
#include "gamelogic/bagtransition.h"
#include "gamelogic/rolenethandler.h"
#include "unit/rolemanager.h"
#include "utility/tlogger.h"


bool CIdipUpdateExpHandler::Process(const rapidjson::Value& roHeadJson, const rapidjson::Value& roBodyJson, CIdipReply& roReply)
{
	std::string strOpenId = JsonUtil::GetJsonString(roBodyJson, "OpenId");
	UINT64 qwRoleID = JsonUtil::GetJsonUint64(roBodyJson, "RoleId");
	INT64 Number	= JsonUtil::GetJsonInt64(roBodyJson, "Num");
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
		roReply.m_nErrCode = IDIP_ERR_NUM_NOT_ENOUGH;
		roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
		return true;
	}
	pRole->DelExp(nNumber);
	pRole->GetNetHandler()->SendExpAndLevelNtf();
	pRole->RecalcuateAttributeAndNotify(RecalAttr_IDIPUpdateExp);

	roReply.m_nErrCode = IDIP_ERR_SUCCESS;
	roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
	return true;
}