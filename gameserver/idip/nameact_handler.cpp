#include "pch.h"
#include "commondef_handler.h"
#include "util/jsonutil.h"
#include "util.h"
#include "unit/rolemanager.h"
#include "unit/role.h"
#include "gamelogic/designationRecord.h"

bool CIdipNameActHandler::Process(const rapidjson::Value& roHeadJson, const rapidjson::Value& roBodyJson, CIdipReply& roReply)
{
	std::string  strNickID = JsonUtil::GetJsonString(roBodyJson, "UId");
	UINT64 NickID = strNickID.empty()?0:convert<UINT64>(strNickID);
	std::string strNameId = JsonUtil::GetJsonString(roBodyJson, "NameId");
	UINT32 NameId = strNameId.empty()?0:convert<UINT32>(strNameId);
	Role* role = RoleManager::Instance()->FindByRoleIDOrNickID(NickID);
	if (NULL == role)
	{
		roReply.m_nErrCode = IDIP_ERR_PLAYER_NOT_EXIST;
		roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
		return true;
	}
	roReply.m_nErrCode = IDIP_ERR_SUCCESS;
	roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
	role->Get<DesignationRecord>()->DelDesignation(NameId);
	return true;
}

