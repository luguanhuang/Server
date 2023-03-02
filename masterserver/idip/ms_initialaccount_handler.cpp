#include "pch.h"
#include "ms_commondef_handler.h"
#include "util/jsonutil.h"
#include "util/dbutil.h"
#include "timeutil.h"
#include "table/globalconfig.h"
#include "guild/guild.h"
#include "guild/guildmgr.h"
#include "account/accountkick.h"
#include "account/punishmgr.h"

INT32 CMSIdipInitialAccountHandler::HandlerRequest(const rapidjson::Value& roHeadJson, const rapidjson::Value& roBodyJson, CIdipReply& roReply)
{
	szOpenId = JsonUtil::GetJsonString(roBodyJson, "OpenId");
	UINT64 qwRoleID = JsonUtil::GetJsonUint64(roBodyJson, "RoleId");

	if (!CRoleSummaryMgr::Instance()->IsValidAcountRole(szOpenId, qwRoleID))
	{
		roReply.m_nErrCode = IDIP_ERR_PLAYER_NOT_EXIST;
		roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
		return IDIP_SERVER_NONE;
	}
	SetRoleID(qwRoleID);

	CRole* pRole = CRoleManager::Instance()->GetByRoleID(qwRoleID);
	if (pRole)
	{
		//踢下线
		if(!CAccountKicker::Instance()->KickAccount(GetRoleID(), KKSG::KICK_NORMAL, NULL, GetID()))
		{
			roReply.m_nErrCode = IDIP_ERR_KICKOUT;
			roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
			return IDIP_SERVER_NONE;
		}
		SetKickAccount();
	}
	return IDIP_SERVER_MS;
}

bool CMSIdipInitialAccountHandler::ReadDataQequest()
{
	RpcM2D_ReadRoleData* rpc = RpcM2D_ReadRoleData::CreateRpc();
	rpc->m_oArg.set_index(GetID());
	rpc->m_oArg.set_roleid(GetRoleID());
	rpc->m_oArg.set_param(ReadAndModify);
	rpc->m_oArg.set_readtype(KKSG::ROLE_DATA_READ_IDIP);
	DBLink::Instance()->SendTo(*rpc);
	return true;
}
bool CMSIdipInitialAccountHandler::KickOutReply()
{
	ReadDataQequest();
	CPunishMgr::Instance()->DoPunishTemp(GetRoleID());
	return false;
}
bool CMSIdipInitialAccountHandler::ModifyMsData(CUserData& roUserData)
{
	return true;
}

bool CMSIdipInitialAccountHandler::SaveDataToDB(const CUserData& roUserData)
{
	RoleAllInfo* pInfo = (RoleAllInfo*)roUserData.m_pUserPtr;

	KKSG::RoleBriefInfo brief;
	brief.set_type(KKSG::RoleType(INT32(pInfo->brief().type())%10));
	brief.set_roleid(pInfo->brief().roleid());
	brief.set_name(pInfo->brief().name());
	brief.set_level(1);

	KKSG::RoleAllInfo oInfo;
	oInfo.mutable_brief()->set_inittime((UINT32)time(NULL));
	oInfo.mutable_brief()->set_type(brief.type());
	oInfo.mutable_brief()->set_name(brief.name());
	oInfo.mutable_brief()->set_accountid(pInfo->brief().accountid());
	oInfo.mutable_brief()->set_roleid(brief.roleid());
	oInfo.mutable_brief()->set_level(1);
	oInfo.mutable_brief()->set_exp(0);
	oInfo.mutable_brief()->set_maxexp(0);
	oInfo.mutable_brief()->set_sceneid( GetGlobalConfig().InitialSceneID);
	oInfo.mutable_brief()->set_nickid(pInfo->brief().nickid());
	oInfo.mutable_extrainfo()->set_lastleveluptime(TimeUtil::GetTime());
	//for (INT32 i = 0; i < VIRTUAL_ITEM_MAX; ++ i)
	//{
	//	UINT64 uValue = 0;
	//	if (i == DIAMOND)
	//	{
	//		uValue =  pInfo->virtualitems_size()>DIAMOND?pInfo->virtualitems(DIAMOND):0;
	//	}
	//	oInfo.add_virtualitems(uValue);
	//}

	RpcM2D_MsSaveRoleData* rpc = RpcM2D_MsSaveRoleData::CreateRpc();
	rpc->m_oArg.set_account(pInfo->brief().accountid());
	rpc->m_oArg.set_roleid(pInfo->brief().roleid());
	rpc->m_oArg.set_slot(MapProfessionToSlot(pInfo->brief().type()));
	rpc->m_oArg.set_param(GetID());
	rpc->m_oArg.set_briefdata(brief.SerializeAsString());
	CRoleDataFields oFields(&oInfo);
	if(!CRoleDataSaveUtil::SaveAll(oFields, rpc->m_oArg))
	{
		LogError("Pack role data failed, roleid=%llu", pInfo->brief().roleid());
		delete rpc;
		return false;
	}
	DBLink::Instance()->SendTo(*rpc);
	return true;
}

bool CMSIdipInitialAccountHandler::OnAsyncReply(const CUserData& roUserData)
{
	if(roUserData.m_pUserPtr == NULL)
	{
		//timeout
		CRpc::ReplyDelayRpc(GetRpcID(), roUserData);
	}
	else
	{		
		//清除邮件 
		CMailMgr::Instance()->DeleteAllMail(GetRoleID(), true);
		Guild *pGuild = CGuildMgr::Instance()->GetGuildByRoleId(GetRoleID());
		if (pGuild)
		{
			pGuild->RemoveMember(GetRoleID());
		}

		EIdipErrCode nErrCode = IDIP_ERR_SUCCESS;
		std::string str;
		CIdipReply oReply(str);
		oReply.m_nErrCode = nErrCode;
		oReply.m_strResult = PackErrResponse(oReply.m_nErrCode);

		CUserData data;
		data.m_pUserPtr = &oReply;
		CRpc::ReplyDelayRpc(GetRpcID(), data);
		IdipLog();
	}
	return true;
}
void CMSIdipInitialAccountHandler::IdipLog()
{
	rapidjson::Document doc;
	std::string s = GetStrJson();
	if (doc.Parse<0>(GetStrJson().c_str()).HasParseError())
	{
		return ;
	}
	TIdipAqDoInitialAccount oLog("IdipAqDoInitialAccount");
	oLog.m_uRoleId = GetRoleID();
	oLog.m_strOpenId = szOpenId;
	oLog.Do(doc["head"], doc["body"]);
}
