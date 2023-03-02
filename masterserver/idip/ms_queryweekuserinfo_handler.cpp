#include "pch.h"
#include "ms_commondef_handler.h"
#include "util/jsonutil.h"
#include "guild/guildactivity.h"
#include "role/rpcm2d_msgetallrole.h"
#include "util/XCommon.h"
#include "util/gametime.h"
#include "table/MultActivityMgr.h"
#include "table/expeditionconfigmgr.h"
#include "define/teamdef.h"


static INT32 GetWeekDataByType(KKSG::WeekReportDataType type, const KKSG::ReportDataRecord& oData)
{
	for (INT32 i = 0; i < oData.wxdata_size(); i ++)
	{
		if (type == oData.wxdata(i).type())
		{
			if (!XCommon::IsWeekSame(GameTime::GetTime(), oData.wxdata(i).lastjointime()))
			{
				return 0;
			}
			return oData.wxdata(i).joincount();
		}
	}
	return 0;
}

bool CMSIdipQueryWeekUserInfoHandler::ReadDataQequest()
{
	RpcM2D_MsGetAllRole *rpc = RpcM2D_MsGetAllRole::CreateRpc();
	rpc->m_oArg.set_account(szOpenId);
	rpc->m_oArg.set_id(GetID());
	rpc->m_oArg.set_serverid(GetServerID());
	if(!DBLink::Instance()->SendTo(*rpc))
	{
		LogWarn("Send rpc to db error");
		return false;
	}
	return true;
}
INT32 CMSIdipQueryWeekUserInfoHandler::HandlerRequest(const rapidjson::Value& roHeadJson, const rapidjson::Value& roBodyJson, CIdipReply& roReply)
{
	szOpenId = JsonUtil::GetJsonString(roBodyJson, "OpenId");
	if(szOpenId.empty())
	{
		roReply.m_nErrCode = IDIP_ERR_INVALID_REQ_NUM;
		roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
		return IDIP_SERVER_NONE;
	}
	return IDIP_SERVER_MS;
}

bool CMSIdipQueryWeekUserInfoHandler::OnAsyncReply(const CUserData& roUserData)
{
	if(roUserData.m_pUserPtr == NULL)
	{
		//timeout
		CRpc::ReplyDelayRpc(GetRpcID(), roUserData);
	}
	else
	{
		KKSG::GetAllRoleRes* poRes = (KKSG::GetAllRoleRes*)roUserData.m_pUserPtr;
		EIdipErrCode nErrCode = IDIP_ERR_UNKNOWN;
		if(poRes->result() == ERR_SUCCESS)
		{
			nErrCode = IDIP_ERR_SUCCESS;
		}
		else if(poRes->result() == ERR_ACCOUNT_NOT_EXIST)
		{
			nErrCode = IDIP_ERR_PLAYER_NOT_EXIST;
		}

		if(nErrCode != IDIP_ERR_SUCCESS)
		{
			std::string str;
			CIdipReply oReply(str);
			oReply.m_nErrCode = nErrCode;
			oReply.m_strResult = PackErrResponse(oReply.m_nErrCode);

			CUserData data;
			data.m_pUserPtr = &oReply;
			CRpc::ReplyDelayRpc(GetRpcID(), data);
			return true;
		}

		JsonBuilder oBuilder;
		oBuilder.AddMember_Int("Result", IDIP_ERR_SUCCESS);
		oBuilder.AddMember_Int("RoleList_count", poRes->roleallinfolist_size());
		rapidjson::Value oRoleList(rapidjson::kArrayType);
		for(INT32 i = 0; i < poRes->roleallinfolist_size(); ++i)
		{
			const KKSG::RoleAllInfo& roInfo = poRes->roleallinfolist(i);
			rapidjson::Value oRole(rapidjson::kObjectType);
			oRole.AddMember("RoleId", ToString(roInfo.brief().roleid()), oBuilder.Allocator());
			oRole.AddMember("RoleName", UrlEncode(roInfo.brief().name()), oBuilder.Allocator());
			oRole.AddMember("RoleUId", ToString(roInfo.brief().nickid()), oBuilder.Allocator());
			INT32 nContri = 0;
			UINT32 PlayerActive = 0;
			Guild* pGuild = CGuildMgr::Instance()->GetGuildByRoleId(roInfo.brief().roleid());
			if (pGuild)
			{
				GuildMember* pMem = pGuild->GetMember(roInfo.brief().roleid());
				if (pMem)
				{
					nContri = pMem->contribute;
				}
				CGuildActivity* pActivity = pGuild->Get<CGuildActivity>();
				PlayerActive = pActivity?pActivity->GetRoleActivity(roInfo.brief().roleid()):0;
			}
			oRole.AddMember("PlayerContribution", nContri, oBuilder.Allocator());
			oRole.AddMember("PlayerRank", roInfo.pkrecord().point(), oBuilder.Allocator());
			oRole.AddMember("PlayerActive", PlayerActive, oBuilder.Allocator());
			oRole.AddMember("PlayerSignA", GetWeekDataByType(KKSG::WeekReportData_GuildSign, roInfo.reportdata()), oBuilder.Allocator());
			oRole.AddMember("PlayerSignB", GetGlobalConfig().GuildSignWeekCount, oBuilder.Allocator());
			oRole.AddMember("WorldBossA", GetWeekDataByType(KKSG::WeekReportData_WorldBoss, roInfo.reportdata()), oBuilder.Allocator());
			oRole.AddMember("WorldBossB", MultActivityMgr::Instance()->GetWeekCount(MULT_ACTIVITY_WORLD_BOSS), oBuilder.Allocator());
			oRole.AddMember("GuildJoinA", GetWeekDataByType(KKSG::WeekReportData_GuildRisk, roInfo.reportdata()), oBuilder.Allocator());
			oRole.AddMember("GuildJoinB", ExpeditionConfigMgr::Instance()->GetType2Count(GUILDCAMP_TEAM_SCENE)*GetGlobalConfig().GuildCampDayID.size(), oBuilder.Allocator());
			oRole.AddMember("GuildRingJoinA", GetWeekDataByType(KKSG::WeekReportData_GuildArena, roInfo.reportdata()), oBuilder.Allocator());
			oRole.AddMember("GuildRingJoinB", MultActivityMgr::Instance()->GetWeekCount(MULT_ACTIVITY_1), oBuilder.Allocator());
			oRole.AddMember("GuildBossJionA", GetWeekDataByType(KKSG::WeekReportData_GuildBoss, roInfo.reportdata()), oBuilder.Allocator());
			oRole.AddMember("GuildBossJionB", MultActivityMgr::Instance()->GetWeekCount(MULT_ACTIVITY_GUILD_BOSS), oBuilder.Allocator());
			oRole.AddMember("ManorFightA", GetWeekDataByType(KKSG::WeekReportData_GuildTerryitory, roInfo.reportdata()), oBuilder.Allocator());
			oRole.AddMember("ManorFightB", MultActivityMgr::Instance()->GetWeekCount(MULT_ACTIVITY_16), oBuilder.Allocator());
			oRoleList.PushBack(oRole, oBuilder.Allocator());
		}
		oBuilder.AddMember_Value("RoleWeekList", oRoleList);

		std::string str = oBuilder.GetString();
		CIdipReply oReply(str);
		oReply.m_nErrCode = IDIP_ERR_SUCCESS;
		CUserData data;
		data.m_pUserPtr = &oReply;
		CRpc::ReplyDelayRpc(GetRpcID(), data);
	}
	return true;
}
