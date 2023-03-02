#include "pch.h"
#include "ms_commondef_handler.h"
#include "util/jsonutil.h"
#include "role/rolemanager.h"
#include "role/role.h"
#include "role/bag.h"
#include "account/accountkick.h"
#include "account/punishmgr.h"

IDIP_HANDLER_MODIFY_IMPLEMETION(DoDelItemX)
INT32 CMSIdipDoDelItemXHandler::HandlerRequest(const rapidjson::Value& roHeadJson, const rapidjson::Value& roBodyJson, CIdipReply& roReply)
{
	m_RoleId = JsonUtil::GetJsonUint64(roBodyJson, "RoleId");
	m_strOpenId = JsonUtil::GetJsonString(roBodyJson, "OpenId");
	m_Type = JsonUtil::GetJsonInt(roBodyJson, "Type");
	m_ItemID = JsonUtil::GetJsonUint(roBodyJson, "ItemId");
	m_ItemUid = JsonUtil::GetJsonUint64(roBodyJson, "ItemUId");
	m_JadeId = JsonUtil::GetJsonUint(roBodyJson, "DragonJadeId");
	if (!m_ItemID || m_Type < DoDelItemType_Fashion || m_Type > DoDelItemType_Jade || !m_ItemUid || (m_Type == DoDelItemType_Jade && !m_JadeId))
	{
		roReply.m_nErrCode = IDIP_ERR_INVALID_PARA;
		roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
		return IDIP_SERVER_NONE;
	}
	if (!CRoleSummaryMgr::Instance()->IsValidAcountRole(m_strOpenId, m_RoleId))
	{
		roReply.m_nErrCode = IDIP_ERR_PLAYER_NOT_EXIST;
		roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
		return IDIP_SERVER_NONE;
	}
	SetRoleID(m_RoleId);
	CRole* pRole = CRoleManager::Instance()->GetByRoleID(m_RoleId);
	if (NULL != pRole)//玩家在线
	{
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

bool CMSIdipDoDelItemXHandler::ModifyMsData(CUserData& roUserData)
{
	RoleAllInfo* pInfo = (RoleAllInfo*)roUserData.m_pUserPtr;
	CBag data(pInfo);
	bool isSuccess = false;
	if (m_Type == DoDelItemType_Fashion)
	{
		isSuccess = data.DelFashion(m_ItemID, m_ItemUid);
	}
	else if (m_Type == DoDelItemType_Item)
	{
		isSuccess = data.DelItemByUid(m_ItemID, m_ItemUid);
	}
	else if (m_Type == DoDelItemType_Emblem)
	{
		isSuccess = data.DelEmblemByUid(m_ItemID, m_ItemUid);
	}
	else if (m_Type == DoDelItemType_Jade)
	{
		isSuccess = data.DelItemJade(m_ItemID, m_ItemUid, m_JadeId);
	}
	if (isSuccess)
	{
		data.Save();
	}
	return isSuccess;
}

void CMSIdipDoDelItemXHandler::IdipLog()
{
	rapidjson::Document doc;
	if (doc.Parse<0>(GetStrJson().c_str()).HasParseError())
	{
		return ;
	}
	TIdipDoDelItemOutBag oLog("IdipDoDelItemOutBag");
	oLog.m_strOpenId = m_strOpenId;
	oLog.m_RoleId = GetRoleID();
	oLog.m_ItemID = m_ItemID;
	oLog.m_ItemUid = m_ItemUid;
	oLog.m_Type = m_Type;
	oLog.m_JadeId = m_JadeId;
	oLog.Do(doc["head"], doc["body"]);
}

bool CMSIdipDoDelItemXHandler::KickOutReply()
{
	ReadDataQequest();
	CPunishMgr::Instance()->DoPunishTemp(GetRoleID());
	return false;//这里要返回false;
}
