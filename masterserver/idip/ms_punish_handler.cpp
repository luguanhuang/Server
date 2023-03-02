#include "pch.h"
#include "ms_commondef_handler.h"
#include "util/jsonutil.h"
#include <time.h>
#include "network/dblink.h"
#include "account/punishmgr.h"
#include "account/ptcm2d_savepunishdata.h"
#include "util/XCommon.h"
#include "loghelper/tlogr.h"
#include "account/accountkick.h"

INT32 CMSIdipPunishHandler::HandlerRequest(const rapidjson::Value& roHeadJson, const rapidjson::Value& roBodyJson, CIdipReply& roReply)
{
	std::string strOpenId = JsonUtil::GetJsonString(roBodyJson, "OpenId");
	UINT64 qwRoleID = JsonUtil::GetJsonUint64(roBodyJson, "RoleId");

	if (!CRoleSummaryMgr::Instance()->IsValidAcountRole(strOpenId, qwRoleID))
	{
		roReply.m_nErrCode = IDIP_ERR_PLAYER_NOT_EXIST;
		roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
		return IDIP_SERVER_NONE;
	}
	SetRoleID(qwRoleID);

	std::string strBanReason("");
	INT32 BanTime = -1;
	INT32 nType = 0;
	switch (GetCmd())
	{
		//·âºÅ;
		case IDIP_DO_BAN_USR_REQ:
		case IDIP_AQ_DO_BAN_USR_REQ:
			{
				strBanReason = JsonUtil::GetJsonString(roBodyJson, "BanReason");
				BanTime = JsonUtil::GetJsonInt(roBodyJson, "BanTime");
				nType = KKSG::PUNISH_USER_LOGIN;
			}
			break;
		case IDIP_DO_MASK_CHAT_REQ:
			{
				strBanReason = JsonUtil::GetJsonString(roBodyJson, "BanChatReason");
				BanTime = JsonUtil::GetJsonInt(roBodyJson, "UnBanTime");
				nType = KKSG::PUNISH_USER_CHAT;
			}
			break;
		case IDIP_AQ_DO_MASK_CHAT_REQ:
			{
				strBanReason = JsonUtil::GetJsonString(roBodyJson, "BanChatReason");
				BanTime = JsonUtil::GetJsonInt(roBodyJson, "BanTime");
				nType = KKSG::PUNISH_USER_CHAT;
			}
		default:
			break;
	}
	if (BanTime < 0 || BanTime > FOREVER_TIME)
	{
		BanTime = FOREVER_TIME;//ÓÀ¾Ã·âºÅÄ¬ÈÏ10Äê
	}
	m_oData.set_account(strOpenId);
	m_oData.set_roleid(qwRoleID);
	m_oData.set_punishdate(UINT32(time(NULL)));
	m_oData.set_punishtype(nType);
	m_oData.set_bantime(BanTime);
	m_oData.set_reason(strBanReason);
	m_oData.set_op(KKSG::PUNISH_DATA_UPATE);

	switch (GetCmd())
	{
		//·âºÅ;
	case IDIP_DO_BAN_USR_REQ:
	case IDIP_AQ_DO_BAN_USR_REQ:
		{
			if (CRoleManager::Instance()->GetByRoleID(qwRoleID))
			{
				if(!CAccountKicker::Instance()->KickAccount(GetRoleID(), KKSG::KICK_NORMAL, NULL, GetID()))
				{
					roReply.m_nErrCode = IDIP_ERR_KICKOUT;
					roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
					return IDIP_SERVER_NONE;
				}
				SetKickAccount();
				return IDIP_SERVER_MS;
			}
			break;
		}
	default:
		break;
	}
	DoPunish(roReply);
	return IDIP_SERVER_NONE;
}
void CMSIdipPunishHandler::DoPunish(CIdipReply& roReply)
{
	CPunishMgr::Instance()->DoPunishUser(GetRoleID(), m_oData);
	roReply.m_nErrCode = IDIP_ERR_SUCCESS;
	roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
	IdipLog();
}
bool CMSIdipPunishHandler::KickOutReply()
{
	std::string str;
	CIdipReply roReply(str);
	DoPunish(roReply);
	CUserData roUserData(UINT32(roReply.m_nErrCode), (void*)&roReply);
	CRpc::ReplyDelayRpc(GetRpcID(), roUserData);
	return true;
}
void CMSIdipPunishHandler::IdipLog()
{
	rapidjson::Document doc;
	std::string s = GetStrJson();
	if (doc.Parse<0>(GetStrJson().c_str()).HasParseError())
	{
		return ;
	}
	std::string strTableName = m_oData.punishtype()==KKSG::PUNISH_USER_LOGIN?"TIdipPunishLoginFlow":"TIdipPunishChatFlow";
	TIdipPunish oLog(strTableName);
	oLog.m_iBanTime = m_oData.bantime();
	oLog.m_uRoleId = GetRoleID();
	oLog.m_strOpenId = m_oData.account();
	oLog.m_strBanReason = m_oData.reason();
	oLog.Do(doc["head"], doc["body"]);
}
