#include "pch.h"
#include "ms_commondef_handler.h"
#include "util/jsonutil.h"
#include "role/bag.h"
#include "guild/guild.h"
#include "guild/guildmgr.h"
#include "marriage/marriage.h"
#include "marriage/marriagemgr.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/stringbuffer.h"

IDIP_HANDLER_READ_IMPLEMETION(QueryRoleInfoX)

void CMSIdipQueryRoleInfoXHandler::BuildMarriageData(UINT64 roleid, std::string& strJsonData)
{
	rapidjson::Document doc;
	if (doc.Parse<0>(strJsonData.c_str()).HasParseError())
	{
		return ;
	}
	Marriage* marriage = MarriageMgr::Instance()->GetMarriage(roleid);
	UINT64 coupleid = 0;
	std::string couplename("0");
	INT32 marriagetime = 0;
	INT32 marriagecount = 0;
	if (marriage)
	{
		coupleid = marriage->GetCoupleID();
		marriagetime = marriage->GetMarriageTime();
		marriagecount = marriage->GetHisMarriageCount();
		if (coupleid)
		{
			CRoleSummary* pSum = CRoleSummaryMgr::Instance()->GetRoleSummary(coupleid);
			couplename = pSum?pSum->GetName():"0";
			marriagecount++;
		}
	}
	rapidjson::Document::AllocatorType &allocator = doc.GetAllocator();  
	rapidjson::Value v(rapidjson::kStringType);
	v.SetString(UrlEncode(couplename).c_str(), allocator);
	doc.AddMember(rapidjson::Value("BuddyName", allocator), v, allocator);

	v.SetString(ToString(coupleid).c_str(), allocator);
	doc.AddMember(rapidjson::Value("BuddyRoleId", allocator), v, allocator);

	rapidjson::Value vv(rapidjson::kNumberType);
	vv.SetInt(marriagetime);
	doc.AddMember(rapidjson::Value("MarryDate", allocator), vv, allocator);

	vv.SetInt(marriagecount);
	doc.AddMember(rapidjson::Value("MarryNum", allocator), vv, allocator);

	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	doc.Accept(writer);

	strJsonData = buffer.GetString();
}
INT32 CMSIdipQueryRoleInfoXHandler::HandlerRequest(const rapidjson::Value& roHeadJson, const rapidjson::Value& roBodyJson, CIdipReply& roReply)
{
	UINT64 roleid = JsonUtil::GetJsonUint64(roBodyJson, "RoleId");
	szOpenId = JsonUtil::GetJsonString(roBodyJson, "OpenId");
	if (!CRoleSummaryMgr::Instance()->IsValidAcountRole(szOpenId, roleid))
	{
		roReply.m_nErrCode = IDIP_ERR_PLAYER_NOT_EXIST;
		roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
		return IDIP_SERVER_NONE;
	}
	SetRoleID(roleid);

	CRole* pRole = CRoleManager::Instance()->GetByRoleID(roleid);
	if (NULL != pRole)//Íæ¼ÒÔÚÏß
	{
		return IDIP_SERVER_GS;
	}
	else
	{
		return IDIP_SERVER_MS;
	}
}
bool CMSIdipQueryRoleInfoXHandler::OnAsyncReply(const CUserData& roUserData)
{
	if(roUserData.m_pUserPtr == NULL)
	{
		//timeout
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
		oBuilder.AddMember_String("RoleName", UrlEncode(pInfo->brief().name()).c_str());
		oBuilder.AddMember_Int("Level", pInfo->brief().level());
		oBuilder.AddMember_Uint("Exp", pInfo->brief().exp());
		oBuilder.AddMember_Int("Physical", GetVirtualItem(*pInfo, FATIGUE));
		oBuilder.AddMember_Uint("RegisterTime", pInfo->brief().inittime());
		oBuilder.AddMember_Uint("LastLogoutTime", pInfo->extrainfo().lastlogouttime());
		oBuilder.AddMember_Uint("OnlineTime", pInfo->brief().onlimetime());
		oBuilder.AddMember_Uint("IsOnline", 1);
		oBuilder.AddMember_Int("DiamondNum", GetVirtualItem(*pInfo, DIAMOND));
		oBuilder.AddMember_Int("GoldNum", GetVirtualItem(*pInfo, GOLD));
		oBuilder.AddMember_Uint("HeartNum", 0);
		Guild* pGuild = CGuildMgr::Instance()->GetGuildByRoleId(pInfo->brief().roleid());
		UINT64 GuildId = pGuild?pGuild->GetID():0;
		std::string GuildName = pGuild?pGuild->GetName():"null";
		oBuilder.AddMember_String("GuildId", ToString(GuildId));
		oBuilder.AddMember_String("GuildName", UrlEncode(GuildName));
		UINT32 wincount = 0;
		UINT32 totalcount = 0;
		if (GetGlobalConfig().PkGetResetLeftTime(pInfo->pkrecord().week(), UINT32(GameTime::GetTime())))
		{
			totalcount = pInfo->pkrecord().histweek().win() + pInfo->pkrecord().histweek().lose() + pInfo->pkrecord().histweek().draw();
			wincount = pInfo->pkrecord().histweek().win();
		}
		UINT32 winrate = totalcount?UINT32(wincount*100/totalcount):0;
		oBuilder.AddMember_Int("LadderCount", totalcount);
		oBuilder.AddMember_Int("LadderWin", winrate);

		std::string str = oBuilder.GetString();
		BuildMarriageData(GetRoleID(), str);
		CIdipReply oReply(str);
		oReply.m_nErrCode = IDIP_ERR_SUCCESS;
		CUserData data;
		data.m_pUserPtr = &oReply;
		CRpc::ReplyDelayRpc(GetRpcID(), data);
	}
	return true;
}

bool CMSIdipQueryRoleInfoXHandler::OnAsyncReplyFromGs(const CUserData& roUserData)
{
	if(roUserData.m_pUserPtr == NULL)
	{
		CRpc::ReplyDelayRpc(GetRpcID(), roUserData);
	}
	else
	{
		if ((KKSG::ErrorCode)roUserData.m_dwUserData == ERR_SUCCESS)
		{
			CIdipReply* poReply = (CIdipReply*)roUserData.m_pUserPtr;
			BuildMarriageData(GetRoleID(), poReply->m_strResult);
			CUserData data;
			data.m_pUserPtr = poReply;
			CRpc::ReplyDelayRpc(GetRpcID(), data);
			return true;
		}
		CRpc::ReplyDelayRpc(GetRpcID(), roUserData);
	}
	return true;
}
