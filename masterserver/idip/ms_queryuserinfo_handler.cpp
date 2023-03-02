#include "pch.h"
#include "ms_commondef_handler.h"
#include "util/jsonutil.h"
#include "role/rpcm2d_msgetallrole.h"
#include "define/itemdef.h"
#include "util.h"
#include "login/rpcm2d_readaccountdata.h"
#include "table/ProfessionMgr.h"
#include "scene/sceneconfig.h"
#include "guild/guild.h"
#include "guild/guildmgr.h"
#include "idutil.h"

#define DEFAULT_HARDLEVEL 1

bool CMSIdipQueryUserInfoHandler::ReadDataQequest()
{
	rapidjson::Document doc;
	std::string s = GetStrJson();
	if (doc.Parse<0>(GetStrJson().c_str()).HasParseError())
	{
		return false;
	}
	std::string strOpenId = JsonUtil::GetJsonString(doc["body"], "OpenId");
	if(strOpenId.empty())
	{
		return false;
	}
	RpcM2D_MsGetAllRole *rpc = RpcM2D_MsGetAllRole::CreateRpc();
	rpc->m_oArg.set_account(strOpenId);
	rpc->m_oArg.set_id(GetID());
	rpc->m_oArg.set_serverid(GetServerID());
	if(!DBLink::Instance()->SendTo(*rpc))
	{
		LogWarn("Send rpc to db error");
		return false;
	}
	return true;
}
INT32 CMSIdipQueryUserInfoHandler::HandlerRequest(const rapidjson::Value& roHeadJson, const rapidjson::Value& roBodyJson, CIdipReply& roReply)
{
	std::string strOpenId = JsonUtil::GetJsonString(roBodyJson, "OpenId");
	if(strOpenId.empty())
	{
		roReply.m_nErrCode = IDIP_ERR_INVALID_REQ_NUM;
		roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
		return IDIP_SERVER_NONE;
	}
	return IDIP_SERVER_MS;
}

bool CMSIdipQueryUserInfoHandler::OnAsyncReply(const CUserData& roUserData)
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
			INT32 nOnLine = (CRoleManager::Instance()->GetByRoleID(roInfo.brief().roleid()) == NULL ? 1 : 0);

			rapidjson::Value oRole(rapidjson::kObjectType);
			oRole.AddMember("RoleId", ToString(roInfo.brief().roleid()), oBuilder.Allocator());
			oRole.AddMember("RoleName", UrlEncode(roInfo.brief().name()), oBuilder.Allocator());

			oRole.AddMember("DiamondNum", GetVirtualItem(roInfo, DIAMOND), oBuilder.Allocator());
			oRole.AddMember("DragonCoin", GetVirtualItem(roInfo, DRAGON_COIN), oBuilder.Allocator());
			oRole.AddMember("GoldNum", GetVirtualItem(roInfo, GOLD), oBuilder.Allocator());
			oRole.AddMember("Physical", GetVirtualItem(roInfo, FATIGUE), oBuilder.Allocator());

			oRole.AddMember("Exp", roInfo.brief().exp(), oBuilder.Allocator());
			oRole.AddMember("Level", roInfo.brief().level(), oBuilder.Allocator());
			oRole.AddMember("RegTime", (UINT32)roInfo.brief().inittime(), oBuilder.Allocator());
			oRole.AddMember("OnlineTime", roInfo.brief().onlimetime(), oBuilder.Allocator());
			oRole.AddMember("LogoutTime", roInfo.extrainfo().lastlogouttime(), oBuilder.Allocator());
			oRole.AddMember("IsOnline", nOnLine, oBuilder.Allocator());
			oRole.AddMember("RegisterSource", 0, oBuilder.Allocator());
			oRole.AddMember("Profession", UrlEncode(CProfessionMgr::Instance()->GetProfessionName(roInfo.brief().type())), oBuilder.Allocator());
			
			INT32 iFight = 0;
			CRoleSummary *pRoleSum = CRoleSummaryMgr::Instance()->GetRoleSummary(roInfo.brief().roleid());
			if (pRoleSum != NULL)
			{
				iFight = pRoleSum->GetPowerPoint();
			}
			oRole.AddMember("Fight", iFight, oBuilder.Allocator());
			oRole.AddMember("ClothFight", 0, oBuilder.Allocator());

			INT32 iMainSceneID = 0;
			INT32 iGulfSceneID = 0;
			std::string szMain("0");
			std::string szGulf("0");
			if (roInfo.has_stages())
			{
				const SceneConf* pRow = NULL;
				for (INT32 i = 0; i < roInfo.stages().sceneid_size(); i ++)
				{
					pRow = CSceneConfig::Instance()->GetSceneConf(roInfo.stages().sceneid(i));
					if (pRow == NULL)
					{
						continue;
					}
					if (pRow->type == KKSG::SCENE_BATTLE && iMainSceneID < roInfo.stages().sceneid(i))
					{
						iMainSceneID = roInfo.stages().sceneid(i);
					}
					else if (pRow->type == KKSG::SCENE_ABYSSS && iGulfSceneID < roInfo.stages().sceneid(i))
					{
						iGulfSceneID = roInfo.stages().sceneid(i);
					}
				}
				if (iMainSceneID > 0)
				{
					pRow = CSceneConfig::Instance()->GetSceneConf(iMainSceneID);
					if (pRow)
					{
						std::stringstream ss;
						ss << iMainSceneID << "_" << pRow->Comment;
						szMain = ss.str();
					}
				}

				if (iGulfSceneID > 0)
				{
					pRow = CSceneConfig::Instance()->GetSceneConf(iGulfSceneID);
					if (pRow)
					{
						std::stringstream ss;
						ss << iGulfSceneID << "_" << pRow->Comment;
						szGulf = ss.str();
					}
				}
			}

			oRole.AddMember("MainPosition", UrlEncode(szMain), oBuilder.Allocator());
			oRole.AddMember("GulfPosition", UrlEncode(szGulf), oBuilder.Allocator());

			INT32 iTowerFloor = 0;
			for (INT32 i = 0; i < roInfo.towerrecord().records_size(); ++ i)
			{
				if (roInfo.towerrecord().records(i).openhardlevel() == DEFAULT_HARDLEVEL)
				{
					iTowerFloor = roInfo.towerrecord().records(i).reachtopfloor();
				}
			}


			oRole.AddMember("TemplePosition", iTowerFloor, oBuilder.Allocator());
			Guild* pGuild = CGuildMgr::Instance()->GetGuildByRoleId(roInfo.brief().roleid());
			UINT64 GuildId = pGuild?pGuild->GetID():0;
			std::string GuildName = pGuild?pGuild->GetName():"null";
			oRole.AddMember("GuildId", ToString(GuildId), oBuilder.Allocator());
			oRole.AddMember("GuildName", UrlEncode(GuildName), oBuilder.Allocator());
			oRole.AddMember("UId", ToString(roInfo.brief().nickid()), oBuilder.Allocator());
			oRole.AddMember("HorseNum", roInfo.petsys().pets_size(), oBuilder.Allocator());
			oRole.AddMember("FairyNum", roInfo.spriterecord().spritedata_size(), oBuilder.Allocator());
			INT32 GuildOnlineNum = pGuild?pGuild->GetOnLineNum():0;
			INT32 GuildTotalNum = pGuild?pGuild->GetMemberCount():0;
			UINT32 GuildLv = pGuild?pGuild->GetLevel():0;
			UINT32 GuildPrestige = pGuild?pGuild->GetPrestige():0;
			INT32 GuildRank = pGuild?pGuild->GetRank():0;
			INT32 GuildPos = pGuild?pGuild->GetPosition(roInfo.brief().roleid()):-1;
			oRole.AddMember("GuildCountOnline", GuildOnlineNum, oBuilder.Allocator());
			oRole.AddMember("GuildCountTatol", GuildTotalNum, oBuilder.Allocator());
			oRole.AddMember("GuildLevel", GuildLv, oBuilder.Allocator());
			oRole.AddMember("GuildPrestige", GuildPrestige, oBuilder.Allocator());
			oRole.AddMember("GuildRank", GuildRank, oBuilder.Allocator());
			oRole.AddMember("PlayerJob", GuildPos, oBuilder.Allocator());
			oRole.AddMember("PlayerRank", roInfo.pkrecord().point(), oBuilder.Allocator());
			oRoleList.PushBack(oRole, oBuilder.Allocator());
		}

		oBuilder.AddMember_Value("RoleList", oRoleList);

		std::string str = oBuilder.GetString();
		CIdipReply oReply(str);
		oReply.m_nErrCode = IDIP_ERR_SUCCESS;
		CUserData data;
		data.m_pUserPtr = &oReply;
		CRpc::ReplyDelayRpc(GetRpcID(), data);
	}
	return true;
}
