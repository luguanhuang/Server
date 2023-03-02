#include "pch.h"
#include "ms_commondef_handler.h"
#include "util/jsonutil.h"
#include "table/PetConfigMgr.h"

IDIP_HANDLER_READ_IMPLEMETION(QueryHorseInfo)

INT32 CMSIdipQueryHorseInfoHandler::HandlerRequest(const rapidjson::Value& roHeadJson, const rapidjson::Value& roBodyJson, CIdipReply& roReply)
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

bool CMSIdipQueryHorseInfoHandler::OnAsyncReply(const CUserData& roUserData)
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
		rapidjson::Value oList(rapidjson::kArrayType);
		const KKSG::PetSysData& roInfo = pInfo->petsys();
		for (INT32 i = 0; i < roInfo.pets_size(); i ++)
		{
			UINT32 Quality = 0;
			std::string strName = "null";
			auto pConf = CPetConfigMgr::Instance()->GetConf(roInfo.pets(i).petid());
			if (pConf)
			{
				Quality = pConf->quality;
				strName = pConf->name;
			}
			rapidjson::Value oPet(rapidjson::kObjectType);
			oPet.AddMember("OpenId", pInfo->brief().accountid(), oBuilder.Allocator());
			oPet.AddMember("UId", pInfo->brief().nickid(), oBuilder.Allocator());
			oPet.AddMember("RoleId", ToString(GetRoleID()), oBuilder.Allocator());
			oPet.AddMember("NickName", UrlEncode(pInfo->brief().name()), oBuilder.Allocator());
			oPet.AddMember("HorseName", UrlEncode(strName), oBuilder.Allocator());
			oPet.AddMember("HorseLevel", roInfo.pets(i).level(), oBuilder.Allocator());
			oPet.AddMember("HorseQual", ToString(Quality), oBuilder.Allocator());
			oPet.AddMember("HorseFeel", roInfo.pets(i).mood(), oBuilder.Allocator());
			oPet.AddMember("IsAct", roInfo.pets(i).uid()==roInfo.fightid()?0:1, oBuilder.Allocator());
			oPet.AddMember("ExpNum", roInfo.pets(i).power(), oBuilder.Allocator());
			oPet.AddMember("HorseUid", ToString(roInfo.pets(i).uid()), oBuilder.Allocator());
			oPet.AddMember("HorseId", roInfo.pets(i).petid(), oBuilder.Allocator());
			oList.PushBack(oPet, oBuilder.Allocator());
		}
		oBuilder.AddMember_Int("Result", KKSG::ERR_SUCCESS);
		oBuilder.AddMember_Uint("HorseList_count", roInfo.pets_size());
		oBuilder.AddMember_Value("HorseList", oList);
		std::string str = oBuilder.GetString();
		CIdipReply oReply(str);
		oReply.m_nErrCode = IDIP_ERR_SUCCESS;
		CUserData data;
		data.m_pUserPtr = &oReply;
		CRpc::ReplyDelayRpc(GetRpcID(), data);
	}
	return true;
}
