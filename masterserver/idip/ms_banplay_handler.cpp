#include "pch.h"
#include "ms_commondef_handler.h"
#include "util/jsonutil.h"

IDIP_HANDLER_MODIFY_IMPLEMETION_EXCEPT_GSREPLAY(BanPlay)
IDIP_DO_OTHER(BanPlay)
INT32 CMSIdipBanPlayHandler::HandlerRequest(const rapidjson::Value& roHeadJson, const rapidjson::Value& roBodyJson, CIdipReply& roReply)
{
	szOpenId = JsonUtil::GetJsonString(roBodyJson, "OpenId");
	UINT64 qwRoleId = JsonUtil::GetJsonUint64(roBodyJson, "RoleId");
	if (!CRoleSummaryMgr::Instance()->IsValidAcountRole(szOpenId, qwRoleId))
	{
		roReply.m_nErrCode = IDIP_ERR_PLAYER_NOT_EXIST;
		roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
		return IDIP_SERVER_NONE;
	}
	SetRoleID(qwRoleId);
	m_nNum = JsonUtil::GetJsonInt(roBodyJson, "BanTime");
	szMailContent = JsonUtil::GetJsonString(roBodyJson, "PromptText");
	m_uItemID =  (GetCmd()==IDIP_AQ_DO_BAN_ALL_PLAY_REQ)?BanPlayTypeALL:JsonUtil::GetJsonUint(roBodyJson, "BanType");

	if (m_uItemID < BanPlayTypeDailyPlay || m_uItemID > BanPlayTypeALL || (m_uItemID > BanPlayTypeMutilActivity && m_uItemID < BanPlayTypeALL))
	{
		roReply.m_nErrCode = IDIP_ERR_INVALID_PARA;
		roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
		return IDIP_SERVER_NONE;
	}
	m_nNum = m_nNum > FOREVER_TIME?FOREVER_TIME:m_nNum;
	CRole* pRole = CRoleManager::Instance()->GetByRoleID(qwRoleId);
	if (NULL == pRole)//不在线ms读取数据扣除;
	{
		return IDIP_SERVER_MS;
	}
	return IDIP_SERVER_GS;
}
static bool ModifyIdipPunishData(RoleAllInfo* pInfo, KKSG::IdipPunishData oData)
{
	KKSG::IdipPunishData* pData = NULL;
	for (INT32 i = 0; i < pInfo->idiprecord().punishinfo_size(); ++ i)
	{
		if (pInfo->idiprecord().punishinfo(i).type() == oData.type())
		{
			pData = pInfo->mutable_idiprecord()->mutable_punishinfo(i);
			break;
		}
	}
	if (NULL == pData)
	{
		pData = pInfo->mutable_idiprecord()->add_punishinfo();
	}
	pData->CopyFrom(oData);
	return true;
}
bool CMSIdipBanPlayHandler::ModifyMsData(CUserData& roUserData)
{
	RoleAllInfo* pInfo = (RoleAllInfo*)roUserData.m_pUserPtr;
	KKSG::IdipPunishData oData;
	oData.set_punishtime(INT32(time(NULL)));
	oData.set_bantime((INT32)m_nNum);
	oData.set_reason(szMailContent);
	switch(m_uItemID)
	{
	case BanPlayTypeDailyPlay:
		{
			oData.set_type(KKSG::PUNISH_USER_DAILY_PLAY);
			ModifyIdipPunishData(pInfo, oData);
			break;
		}
	case BanPlayTypeMutilActivity:
		{
			oData.set_type(KKSG::PUNISH_USER_MULTI_ACTIVITY);
			ModifyIdipPunishData(pInfo, oData);
			break;
		}
	case BanPlayTypeALL:
		{
			oData.set_type(KKSG::PUNISH_USER_DAILY_PLAY);
			ModifyIdipPunishData(pInfo, oData);
			oData.set_type(KKSG::PUNISH_USER_MULTI_ACTIVITY);
			ModifyIdipPunishData(pInfo, oData);
			break;
		}
	default:
		break;
	}
	return true;
}
bool CMSIdipBanPlayHandler::OnAsyncReplyFromGs(const CUserData& roUserData)
{
	CRpc::ReplyDelayRpc(GetRpcID(), roUserData);
	if (roUserData.m_pUserPtr && (KKSG::ErrorCode)roUserData.m_dwUserData == ERR_SUCCESS)
	{
		CRole* pRole = CRoleManager::Instance()->GetByRoleID(GetRoleID());
		if (pRole)
		{
			KKSG::IdipPunishData oData;
			oData.set_punishtime(INT32(time(NULL)));
			oData.set_bantime((INT32)m_nNum);
			oData.set_reason(szMailContent);
			switch(m_uItemID)
			{
			case BanPlayTypeDailyPlay:
				{
					oData.set_type(KKSG::PUNISH_USER_DAILY_PLAY);
					pRole->SetIdipPunishData(oData);
					break;
				}
			case BanPlayTypeMutilActivity:
				{
					oData.set_type(KKSG::PUNISH_USER_MULTI_ACTIVITY);
					pRole->SetIdipPunishData(oData);
					break;
				}
			case BanPlayTypeALL:
				{
					oData.set_type(KKSG::PUNISH_USER_DAILY_PLAY);
					pRole->SetIdipPunishData(oData);
					oData.set_type(KKSG::PUNISH_USER_MULTI_ACTIVITY);
					pRole->SetIdipPunishData(oData);
					break;
				}
			default:
				break;
			}
		}
		IdipLog();
	}
	return true;
}
void CMSIdipBanPlayHandler::IdipLog()
{
	rapidjson::Document doc;
	std::string s = GetStrJson();
	if (doc.Parse<0>(GetStrJson().c_str()).HasParseError())
	{
		return ;
	}
	std::string strTableName = GetCmd()==IDIP_AQ_DO_BAN_ALL_PLAY_REQ?"TIdipAqDoBanAllPlayFlow":"TIdipAqDoBanPlayFlow";
	TIdipBanPlay oLog(strTableName);
	oLog.m_iBanTime = m_nNum;
	oLog.m_uBanType = m_uItemID;
	oLog.m_uRoleId = GetRoleID();
	oLog.m_strOpenId = szOpenId;
	oLog.m_strPromptText = szMailContent;
	oLog.Do(doc["head"], doc["body"]);
}
