#include "pch.h"
#include "ms_commondef_handler.h"
#include "util/jsonutil.h"
#include "role/bag.h"
#include "guild/guildmgr.h"
#include "guild/guild.h"

IDIP_HANDLER_READ_IMPLEMETION(AqQueryOpenidInfo)
INT32 CMSIdipAqQueryOpenidInfoHandler::HandlerRequest(const rapidjson::Value& roHeadJson, const rapidjson::Value& roBodyJson, CIdipReply& roReply)
{
	UINT64 roleid = JsonUtil::GetJsonUint64(roBodyJson, "RoleId");
	std::string strOpenId = JsonUtil::GetJsonString(roBodyJson, "OpenId");
	if (!CRoleSummaryMgr::Instance()->IsValidAcountRole(strOpenId, roleid))
	{
		roReply.m_nErrCode = IDIP_ERR_PLAYER_NOT_EXIST;
		roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
		return IDIP_SERVER_NONE;
	}
	SetRoleID(roleid);
	return IDIP_SERVER_MS;
}

bool CMSIdipAqQueryOpenidInfoHandler::OnAsyncReply(const CUserData& roUserData)
{
	if (roUserData.m_pUserPtr == NULL)
	{
		CRpc::ReplyDelayRpc(GetRpcID(), roUserData);
	}
	else
	{
		if(roUserData.m_dwUserData != KKSG::ERR_SUCCESS)
		{
			std::string str;
			CIdipReply oReply(str);
			oReply.m_nErrCode = IDIP_ERR_UNKNOWN;
			oReply.m_strResult = PackErrResponse(oReply.m_nErrCode);
			CUserData data;
			data.m_pUserPtr = &oReply;
			CRpc::ReplyDelayRpc(GetRpcID(), data);
			return true;
		}
		RoleAllInfo* pInfo = (RoleAllInfo*)roUserData.m_pUserPtr;
		JsonBuilder oBuilder;
		oBuilder.AddMember_Int("Result", IDIP_ERR_SUCCESS);
		oBuilder.AddMember_String("RoleName", UrlEncode(pInfo->brief().name()).c_str());

		oBuilder.AddMember_Int("GoldNum", GetVirtualItem(*pInfo, GOLD));
		oBuilder.AddMember_Int("DiamondNum", GetVirtualItem(*pInfo, DIAMOND));
		oBuilder.AddMember_Int("DragonCoin", GetVirtualItem(*pInfo, DRAGON_COIN));
		oBuilder.AddMember_Int("HonourNum", GetVirtualItem(*pInfo, ARENA_COIN));

		INT32 nContri = 0;
		Guild* pGuild = CGuildMgr::Instance()->GetGuildByRoleId(GetRoleID());
		if (pGuild)
		{
			GuildMember* pMem = pGuild->GetMember(GetRoleID());
			if (pMem)
			{
				nContri = pMem->contribute;
			}
		}

		oBuilder.AddMember_Int("PartyContribution", nContri);
		CBag oData(pInfo);
		oBuilder.AddMember_Int("GoddessTears", oData.CountItem(GODDNESS_TEARS_ID));
		oBuilder.AddMember_Int("LadderScore", pInfo->pkrecord().point());
		std::string str = oBuilder.GetString();
		CIdipReply oReply(str);
		oReply.m_nErrCode = IDIP_ERR_SUCCESS;
		CUserData data;
		data.m_pUserPtr = &oReply;
		CRpc::ReplyDelayRpc(GetRpcID(), data);
	}
	return true;
}

