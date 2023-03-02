#include "pch.h"
#include "ms_commondef_handler.h"
#include "util/jsonutil.h"
#include "config.h"
#include "role/rolemanager.h"
#include "guild/guildmgr.h"
#include "guild/guild.h"
#include "idip/idip.h"
#include "role/rolesummary.h"

INT32 CMSIdipQueryGuildNoticeHandler::HandlerRequest(const rapidjson::Value& roHeadJson, const rapidjson::Value& roBodyJson, CIdipReply& roReply)
{
	UINT64 qwRoleID = JsonUtil::GetJsonUint64(roBodyJson, "RoleId");
	std::string strOpenId = JsonUtil::GetJsonString(roBodyJson, "OpenId");
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

	GuildMember* pMem = pGuild->GetMember(qwRoleID);
	if (NULL == pMem)
	{
		roReply.m_nErrCode = IDIP_ERR_INVALID_GUILDID;
		roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
		return IDIP_SERVER_NONE;
	}
	roReply.m_nErrCode = IDIP_ERR_SUCCESS;
	JsonBuilder oBuilder;
	oBuilder.AddMember_String("RoleName", UrlEncode(pSum->GetName()).c_str());
	oBuilder.AddMember_Int("Level", pSum->GetLevel());
	oBuilder.AddMember_String("PartyId", ToString(pGuild->GetID()));
	oBuilder.AddMember_Int("PartyIdentity", pMem->position);
	oBuilder.AddMember_String("PartyNotice", UrlEncode(pGuild->GetAnnoucement()).c_str());
	roReply.m_strResult = oBuilder.GetString();

	return IDIP_SERVER_NONE;
}
