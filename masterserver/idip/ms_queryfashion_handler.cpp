#include "pch.h"
#include "ms_commondef_handler.h"
#include "util/jsonutil.h"
#include "role/rolemanager.h"
#include "table/ItemConfigMgr.h"

IDIP_HANDLER_READ_IMPLEMETION(QueryFashion)
INT32 CMSIdipQueryFashionHandler::HandlerRequest(const rapidjson::Value& roHeadJson, const rapidjson::Value& roBodyJson, CIdipReply& roReply)
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
static std::string GetName(std::string name)
{
	return name.empty()?"null":name;
}
bool CMSIdipQueryFashionHandler::OnAsyncReply(const CUserData& roUserData)
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
		const KKSG::FashionRecord &roFashion = pInfo->fashionrecord();
		JsonBuilder oBuilder;
		UINT32 FashionList_count = 0;
		rapidjson::Value oList(rapidjson::kArrayType);
		for (int i = 0; i < roFashion.bodyfashion_size(); ++i)
		{
			KKSG::FashionData rItem = roFashion.bodyfashion(i);
			if (rItem.itemid())
			{
				rapidjson::Value item(rapidjson::kObjectType);
				item.AddMember("Position", 0, oBuilder.Allocator());
				item.AddMember("FashionId", rItem.itemid(), oBuilder.Allocator());
				item.AddMember("FashionName", UrlEncode(GetName(CItemConfig::Instance()->GetItemName(rItem.itemid(), pInfo->brief().type()%10))), oBuilder.Allocator());
				item.AddMember("FashionUId", ToString(rItem.uid()), oBuilder.Allocator());
				oList.PushBack(item, oBuilder.Allocator());
				FashionList_count ++;
			}
		}
		for (int i = 0; i < roFashion.bagfashion_size(); ++i)
		{
			KKSG::FashionData rItem = roFashion.bagfashion(i);
			rapidjson::Value item(rapidjson::kObjectType);
			item.AddMember("Position", 1, oBuilder.Allocator());
			item.AddMember("FashionId", rItem.itemid(), oBuilder.Allocator());
			item.AddMember("FashionName", UrlEncode(GetName(CItemConfig::Instance()->GetItemName(rItem.itemid(), pInfo->brief().type()%10))), oBuilder.Allocator());
			item.AddMember("FashionUId", ToString(rItem.uid()), oBuilder.Allocator());
			oList.PushBack(item, oBuilder.Allocator());
			FashionList_count ++;
		}
		oBuilder.AddMember_Uint("FashionList_count", FashionList_count);
		oBuilder.AddMember_Value("FashionList", oList);

		std::string str = oBuilder.GetString();
		CIdipReply oReply(str);
		oReply.m_nErrCode = IDIP_ERR_SUCCESS;
		CUserData data;
		data.m_pUserPtr = &oReply;
		CRpc::ReplyDelayRpc(GetRpcID(), data);
	}
	return true;
}
