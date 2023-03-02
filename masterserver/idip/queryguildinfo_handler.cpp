#include "pch.h"
#include "ms_commondef_handler.h"
#include "util/jsonutil.h"
#include "config.h"
#include "role/rolemanager.h"
#include "guild/guildmgr.h"
#include "guild/guild.h"
#include "idip/idip.h"

INT32 CMSIdipQueryGuildInfoHandler::HandlerRequest(const rapidjson::Value& roHeadJson, const rapidjson::Value& roBodyJson, CIdipReply& roReply)
{
	UINT64 uGuildId = JsonUtil::GetJsonUint64(roBodyJson, "PartyId");
	Guild *pGuild = CGuildMgr::Instance()->GetGuild(uGuildId);
	if (pGuild == NULL)
	{
		roReply.m_nErrCode = IDIP_ERR_INVALID_GUILDID;
		roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
		return IDIP_SERVER_NONE;
	}
	roReply.m_nErrCode = IDIP_ERR_SUCCESS;
	JsonBuilder oBuilder;
	oBuilder.AddMember_Int("Result", roReply.m_nErrCode);
	oBuilder.AddMember_String("RetMsg", GetIdipErrDesc(	roReply.m_nErrCode).c_str());
	oBuilder.AddMember_Int("PartyArea", MSConfig::Instance()->GetServerID());//公会整体的数据用config;
	oBuilder.AddMember_String("PartyName", UrlEncode(pGuild->GetName()).c_str());
	oBuilder.AddMember_Int("PartyLevel", pGuild->GetLevel());
	oBuilder.AddMember_Int("PartyMemNum", pGuild->GetMemberCount());
	oBuilder.AddMember_String("PartyLeaderId", ToString(pGuild->GetLeaderID()).c_str());
	oBuilder.AddMember_String("PartyLeaderName", UrlEncode(pGuild->GetLeaderName()).c_str());

	const std::map<UINT64, GuildMember>& members = pGuild->GetGuildMember();
	oBuilder.AddMember_Uint("PartyMemList_count", members.size());
	rapidjson::Value oList(rapidjson::kArrayType);
	for (auto i = members.begin(); i != members.end(); ++i)
	{
		CRoleSummary *pRoleSum = CRoleSummaryMgr::Instance()->GetRoleSummary(i->first);
		if (pRoleSum != NULL)
		{
			rapidjson::Value oMem(rapidjson::kObjectType);
			oMem.AddMember("PartyMemId", ToString(i->first), oBuilder.Allocator());
			oMem.AddMember("PartyMemName", UrlEncode(pRoleSum->GetName()), oBuilder.Allocator());
			oMem.AddMember("RoleLevel", pRoleSum->GetLevel(), oBuilder.Allocator());
			oList.PushBack(oMem, oBuilder.Allocator());
			if (i->second.position == GPOS_LEADER)
			{
				oBuilder.AddMember_Int("PartyLeaderLevel", pRoleSum->GetLevel());
			}
		}
	}

	oBuilder.AddMember_Value("PartyMemList", oList);
	roReply.m_strResult = oBuilder.GetString();

	return IDIP_SERVER_NONE;
}
