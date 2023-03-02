#include "pch.h"
#include "ms_commondef_handler.h"
#include "util/jsonutil.h"
#include "account/punishmgr.h"
#include "guild/guildmgr.h"
#include "guild/guild.h"
#include "loghelper/tlogr.h"

INT32 CMSIdipBanOfflineRankHandler::HandlerRequest(const rapidjson::Value& roHeadJson, const rapidjson::Value& roBodyJson, CIdipReply& roReply)
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
	UINT32 uClearRanking = JsonUtil::GetJsonUint(roBodyJson, "ClearRanking");
	UINT32 uRankingType = JsonUtil::GetJsonUint(roBodyJson, "RankingType");
	UINT32 iBanTime = JsonUtil::GetJsonInt(roBodyJson, "BanTime");
	std::string strBanReason = JsonUtil::GetJsonString(roBodyJson, "BanReason");
	bool isClear = uClearRanking==1?true:false;
	if (iBanTime <= 0)
	{
		roReply.m_nErrCode = IDIP_ERR_INVALID_PARA;
		roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
		return IDIP_SERVER_NONE;
	}
	iBanTime = iBanTime > FOREVER_TIME?FOREVER_TIME:iBanTime;
	KKSG::PunishData oData;
	oData.set_account(strOpenId);
	oData.set_roleid(qwRoleID);
	oData.set_punishdate(UINT32(time(NULL)));
	oData.set_punishtype(uRankingType);
	oData.set_bantime(iBanTime);
	oData.set_reason(strBanReason);
	oData.set_op(KKSG::PUNISH_DATA_UPATE);
	std::string strTableName;
	if (GetCmd() == IDIP_AQ_DO_BAN_RANKING_REQ)
	{
		if (!CPunishMgr::Instance()->PunishRank(qwRoleID, oData, isClear))
		{	
			roReply.m_nErrCode = IDIP_ERR_INVALID_PARA;
			roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
			return IDIP_SERVER_NONE;
		}
		strTableName = "TIdipAqDoBanRankingFlow";
	}
	else
	{
		if (!CPunishMgr::Instance()->PunishOfflineRank(qwRoleID, oData, isClear))
		{	
			roReply.m_nErrCode = IDIP_ERR_INVALID_PARA;
			roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
			return IDIP_SERVER_NONE;
		}
		strTableName = "TIdipAqDoBanRankingOfflineFlow";
	}

	TIdipBanRank oLog(strTableName);
	oLog.m_uClearRanking = uClearRanking;
	oLog.m_uRankingType = uRankingType;
	oLog.m_iBanTime = iBanTime;
	oLog.m_strBanReason = strBanReason;
	oLog.Do(roHeadJson, roBodyJson);

	roReply.m_nErrCode = IDIP_ERR_SUCCESS;
	roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
	return IDIP_SERVER_NONE;
}
