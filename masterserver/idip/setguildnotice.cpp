#include "pch.h"
#include "ms_commondef_handler.h"
#include "util/jsonutil.h"
#include "config.h"
#include "role/rolemanager.h"
#include "guild/guildmgr.h"
#include "guild/guild.h"
#include "idip/idip.h"
#include "role/rolesummary.h"

INT32 CMSIdipSetGuildNoticeHandler::HandlerRequest(const rapidjson::Value& roHeadJson, const rapidjson::Value& roBodyJson, CIdipReply& roReply)
{
	UINT64 uGuildId = JsonUtil::GetJsonUint64(roBodyJson, "PartyId");
	UINT64 qwRoleID = JsonUtil::GetJsonUint64(roBodyJson, "RoleId");
	std::string strOpenId = JsonUtil::GetJsonString(roBodyJson, "OpenId");
	std::string strContent = JsonUtil::GetJsonString(roBodyJson, "Content");
	UINT32 uMsgType = JsonUtil::GetJsonUint(roBodyJson, "MsgType");
	if (!CRoleSummaryMgr::Instance()->IsValidAcountRole(strOpenId, qwRoleID))
	{
		roReply.m_nErrCode = IDIP_ERR_PLAYER_NOT_EXIST;
		roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
		return IDIP_SERVER_NONE;
	}
	CRoleSummary* pSum = CRoleSummaryMgr::Instance()->GetRoleSummary(qwRoleID);
	Guild* pGuild = CGuildMgr::Instance()->GetGuildByRoleId(qwRoleID);
	if (NULL == pGuild || NULL == pSum)
	{
		roReply.m_nErrCode = IDIP_ERR_NOTJOIN_GUILD;
		roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
		return IDIP_SERVER_NONE;
	}

	if (pGuild->GetID() != uGuildId)
	{
		roReply.m_nErrCode = IDIP_ERR_INVALID_GUILDID;
		roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
		return IDIP_SERVER_NONE;
	}

	if (uMsgType != 1 && uMsgType != 99)
	{
		roReply.m_nErrCode = IDIP_ERR_INVALID_PARA;
		roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
		return IDIP_SERVER_NONE;
	}

	pGuild->SetAnnoucement(strContent);
	roReply.m_nErrCode = IDIP_ERR_SUCCESS;
	roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
	return IDIP_SERVER_NONE;
}

