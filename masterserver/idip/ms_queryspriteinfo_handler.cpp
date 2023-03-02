#include "pch.h"
#include "ms_commondef_handler.h"
#include "util/jsonutil.h"
#include "table/SpriteConfigMgr.h"

IDIP_HANDLER_READ_IMPLEMETION(QuerySpriteInfo)

INT32 CMSIdipQuerySpriteInfoHandler::HandlerRequest(const rapidjson::Value& roHeadJson, const rapidjson::Value& roBodyJson, CIdipReply& roReply)
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
	m_id1 = JsonUtil::GetJsonInt(roBodyJson, "PageNo");
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
static INT32 GetInFightIndex(const KKSG::SpriteRecord& roInfo, UINT64 uid)
{
	for (INT32 i = 0; i < roInfo.infight_size(); i ++)
	{
		if (roInfo.infight(i) == uid)
		{
			return i;
		}
	}
	return -1;
}
bool CMSIdipQuerySpriteInfoHandler::OnAsyncReply(const CUserData& roUserData)
{
	if(roUserData.m_pUserPtr == NULL)
	{
		//timeout
		CRpc::ReplyDelayRpc(GetRpcID(), roUserData);
	}
	else
	{		
		std::string str;
		CIdipReply oReply(str);
		CUserData data;
		data.m_pUserPtr = &oReply;
		if(roUserData.m_dwUserData != KKSG::ERR_SUCCESS)
		{
			oReply.m_nErrCode = IDIP_ERR_UNKNOWN;
			oReply.m_strResult = PackErrResponse(oReply.m_nErrCode);
			CRpc::ReplyDelayRpc(GetRpcID(), data);
			return true;
		}
		INT32 iStart = 0;
		INT32 iEnd = 0;
		RoleAllInfo* pInfo = (RoleAllInfo*)roUserData.m_pUserPtr;
		JsonBuilder oBuilder;
		rapidjson::Value oList(rapidjson::kArrayType);
		const KKSG::SpriteRecord& roInfo = pInfo->spriterecord();


		if (!GetPageNo(roInfo.spritedata_size(), m_id1, MAX_FAIRYLIST_NUM, iStart, iEnd))
		{
			oReply.m_nErrCode = IDIP_ERR_INVALID_PARA;
			oReply.m_strResult = PackErrResponse(oReply.m_nErrCode);
			CRpc::ReplyDelayRpc(GetRpcID(), data);
			return true;
		}
		UINT32 count = 0;
		for (INT32 i = 0; i < roInfo.spritedata_size(); ++ i)
		{
			if (i < iStart || i >= iEnd)
			{
				continue;
			}
			UINT32 Quality = 0;
			std::string strName = "null";
			auto pConf = CSpriteConfigMgr::Instance()->GetSpriteConf(roInfo.spritedata(i).spriteid());
			if (pConf)
			{
				Quality = pConf->SpriteQuality;
				strName = pConf->SpriteName;
			}
			rapidjson::Value stprite(rapidjson::kObjectType);
			stprite.AddMember("OpenId", pInfo->brief().accountid(), oBuilder.Allocator());
			stprite.AddMember("UId", pInfo->brief().nickid(), oBuilder.Allocator());
			stprite.AddMember("RoleId", ToString(GetRoleID()), oBuilder.Allocator());
			stprite.AddMember("NickName", UrlEncode(pInfo->brief().name()), oBuilder.Allocator());
			stprite.AddMember("FairyName", UrlEncode(strName), oBuilder.Allocator());
			stprite.AddMember("FairyLevel", roInfo.spritedata(i).level(), oBuilder.Allocator());
			stprite.AddMember("FairyQual", ToString(Quality), oBuilder.Allocator());
			INT32 iIndex = GetInFightIndex(roInfo, roInfo.spritedata(i).uid());
			stprite.AddMember("UpDown", iIndex>=0?0:1, oBuilder.Allocator());
			stprite.AddMember("IsCaptain", iIndex==0?0:1, oBuilder.Allocator());
			stprite.AddMember("ExpNum", roInfo.spritedata(i).powerpoint(), oBuilder.Allocator());
			stprite.AddMember("FairyUid", ToString(roInfo.spritedata(i).uid()), oBuilder.Allocator());
			stprite.AddMember("FairyId", roInfo.spritedata(i).spriteid(), oBuilder.Allocator());
			oList.PushBack(stprite, oBuilder.Allocator());
		}
		oBuilder.AddMember_Int("Result", KKSG::ERR_SUCCESS);
		oBuilder.AddMember_Uint("FairyList_count", oList.Size());
		oBuilder.AddMember_Value("FairyList", oList);
		INT32 TotalPageNo = (roInfo.spritedata_size() + MAX_FAIRYLIST_NUM -1) / MAX_FAIRYLIST_NUM;
		oBuilder.AddMember_Int("TotalPageNo", TotalPageNo);
		oReply.m_nErrCode = IDIP_ERR_SUCCESS;
		oReply.m_strResult = oBuilder.GetString();
		CRpc::ReplyDelayRpc(GetRpcID(), data);
	}
	return true;
}
