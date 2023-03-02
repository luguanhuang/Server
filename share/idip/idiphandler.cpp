#include "pch.h"
#include "idip/idiphandler.h"
#include "util/jsonutil.h"
#include "idip/idiperrdesc.h"


CIdipHandler::CIdipHandler()
:m_dwID(0)
,m_nCmd(0)
,m_dwRpcID(0)
,m_szJson("")
,m_dwRoleID(0)
,m_nServerType(IDIP_SERVER_NONE)
,m_bKickAccount(false)
,m_ServerID(0)
{
}

CIdipHandler::CIdipHandler(UINT32 dwID, INT32 nCmd)
:m_dwID(dwID)
,m_nCmd(nCmd)
,m_dwRpcID(0)
,m_szJson("")
,m_dwRoleID(0)
,m_nServerType(IDIP_SERVER_NONE)
,m_bKickAccount(false)
,m_ServerID(0)
{
}

CIdipHandler::~CIdipHandler()
{
}

std::string PackErrResponse(EIdipErrCode nErrCode)
{
	JsonBuilder oBuilder;
	oBuilder.AddMember_Int("Result", nErrCode);
	oBuilder.AddMember_String("RetMsg", GetIdipErrDesc(nErrCode).c_str());
	return oBuilder.GetString();
}

bool GetItemChangeList(const rapidjson::Value& roMember, std::vector<ItemDesc>& items)
{
	items.clear();
	if (roMember.IsNull() || !roMember.IsArray())
	{
		return false;
	}

	for (UINT32 i = 0; i < roMember.Size(); i ++)
	{
		UINT64 uItemId = JsonUtil::GetJsonUint64(roMember[i], "ItemId");
		UINT32 uItemNum = JsonUtil::GetJsonUint(roMember[i], "ItemNum");
		ItemDesc pdesc(uItemId, uItemNum);
		items.push_back(pdesc);
	}
	return true;
}
