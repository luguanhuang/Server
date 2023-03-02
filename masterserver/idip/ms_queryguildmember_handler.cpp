#include "pch.h"
#include "ms_commondef_handler.h"
#include "util/jsonutil.h"
#include "config.h"
#include "role/rolemanager.h"
#include "guild/guildmgr.h"
#include "guild/guild.h"
#include "idip/idip.h"
#include "role/rolesummary.h"
#include "role/rolemanager.h"
#include "guild/guildactivity.h"

INT32 CMSIdipQueryGuildMemberInfoHandler::HandlerRequest(const rapidjson::Value& roHeadJson, const rapidjson::Value& roBodyJson, CIdipReply& roReply)
{
	UINT64 uGuildId = JsonUtil::GetJsonUint64(roBodyJson, "GuildId");
	UINT32 uPageNo = JsonUtil::GetJsonUint(roBodyJson, "PageNo");
	Guild *pGuild = CGuildMgr::Instance()->GetGuild(uGuildId);
	if (pGuild == NULL)
	{
		roReply.m_nErrCode = IDIP_ERR_INVALID_GUILDID;
		roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
		return IDIP_SERVER_NONE;
	}
	const std::map<UINT64, GuildMember>& members = pGuild->GetGuildMember();
	INT32 iStart = (uPageNo - 1) * MAX_GUILDMEMBERLIST_NUM;
	if (iStart < 0 || iStart >= (INT32)members.size())
	{
		roReply.m_nErrCode = IDIP_ERR_INVALID_PARA;
		roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
		return IDIP_SERVER_NONE;
	}
	INT32 iTotalPageNo = (members.size() + MAX_GUILDMEMBERLIST_NUM -1) / MAX_GUILDMEMBERLIST_NUM;
	INT32 iEnd = members.size() < (uPageNo * MAX_GUILDMEMBERLIST_NUM) ? members.size() : uPageNo * 	MAX_GUILDMEMBERLIST_NUM;
	rapidjson::Value oRoleList(rapidjson::kArrayType);
	UINT32 GuildMemberList_count = 0;
	JsonBuilder oBuilder;

	auto iter = members.begin();
	for (INT32 cnt = 0; cnt < iStart && iter != members.end(); ++cnt)
	{
		++iter;
	}

	for (INT32 i = iStart; i < iEnd && iter != members.end(); ++ i)
	{
		CRoleSummary* pSum = CRoleSummaryMgr::Instance()->GetRoleSummary(iter->first);
		std::string strRoleName = pSum?pSum->GetName():"null";
		std::string strOpenId = pSum?pSum->GetAccount():"null";
		INT32 iPro = pSum?pSum->GetProfession():0;
		INT32 iLevel = pSum?pSum->GetLevel():0;
		INT32 iPowerPoint = pSum?pSum->GetPowerPoint():0;
		INT32 iOnline = CRoleManager::Instance()->GetByRoleID(iter->first)!=NULL?1:0;
		rapidjson::Value oRole(rapidjson::kObjectType);
		oRole.AddMember("RoleId", ToString(iter->first), oBuilder.Allocator());
		oRole.AddMember("RoleName", UrlEncode(CRoleSummaryMgr::Instance()->GetRoleName(iter->first)), oBuilder.Allocator());
		oRole.AddMember("Position", iter->second.position, oBuilder.Allocator());
		oRole.AddMember("Job", iPro, oBuilder.Allocator());
		oRole.AddMember("Level", iLevel, oBuilder.Allocator());
		oRole.AddMember("Fight", iPowerPoint, oBuilder.Allocator());
		oRole.AddMember("Contribute", iter->second.contribute, oBuilder.Allocator());
		oRole.AddMember("IsOnline", iOnline, oBuilder.Allocator());
		oRole.AddMember("OpenId", strOpenId, oBuilder.Allocator());
		CGuildActivity* pActivity = pGuild->Get<CGuildActivity>();
		UINT32 ActiveLevel = pActivity?pActivity->GetRoleActivity(iter->first):0;
		oRole.AddMember("ActiveLevel", ActiveLevel, oBuilder.Allocator());
		oRoleList.PushBack(oRole, oBuilder.Allocator());
		GuildMemberList_count ++;
		++iter;
	}
	roReply.m_nErrCode = IDIP_ERR_SUCCESS;
	oBuilder.AddMember_Int("Result", roReply.m_nErrCode);
	oBuilder.AddMember_Uint("GuildMemberList_count", GuildMemberList_count);
	oBuilder.AddMember_Value("GuildMemberList", oRoleList);
	oBuilder.AddMember_Int("TotalPageNo", iTotalPageNo);
	roReply.m_strResult = oBuilder.GetString();

	return IDIP_SERVER_NONE;
}
