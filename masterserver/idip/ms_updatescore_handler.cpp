#include "pch.h"
#include "ms_commondef_handler.h"
#include "util/jsonutil.h"
#include "role/bag.h"
#include "table/globalconfig.h"
#include "guild/guildmgr.h"
#include "guild/guild.h"
#include "account/accountkick.h"

//不在线时，MS读取roleallinfo修改游戏数据，guildmgr修改贡献值;
//在线时，GS修改游戏数据，MS中guildmgr修改贡献值;

IDIP_HANDLER_MODIFY_IMPLEMETION_EXCEPT_GSREPLAY(UpdateScore)
IDIP_DO_OTHER(UpdateScore)
INT32 CMSIdipUpdateScoreHandler::HandlerRequest(const rapidjson::Value& roHeadJson, const rapidjson::Value& roBodyJson, CIdipReply& roReply)
{
	UINT64 roleid = JsonUtil::GetJsonUint64(roBodyJson, "RoleId");
	szOpenId = JsonUtil::GetJsonString(roBodyJson, "OpenId");
	m_uItemID	= JsonUtil::GetJsonUint(roBodyJson, "ScoreType");
	
	if (GetCmd() == IDIP_AQ_DO_CLEAR_SCORE_REQ) //清零分数
	{
		UINT32 uIsClear = JsonUtil::GetJsonUint(roBodyJson, "IsClear");
		if (uIsClear != 1)
		{
			roReply.m_nErrCode = IDIP_ERR_INVALID_PARA;
			roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
			return IDIP_SERVER_NONE;
		}
	}
	else //设置分数
	{
		m_nNum = JsonUtil::GetJsonInt(roBodyJson, "Num");
		if (m_nNum < 0)
		{
			roReply.m_nErrCode = IDIP_ERR_INVALID_PARA;
			roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
			return IDIP_SERVER_NONE;
		}
	}
	if (!m_uItemID || m_uItemID > SCORE_TYPE_ALL || (m_uItemID > SCORE_TYPE_ARENA_POINT && m_uItemID < SCORE_TYPE_ALL))
	{
		roReply.m_nErrCode = IDIP_ERR_INVALID_PARA;
		roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
		return IDIP_SERVER_NONE;
	}
	if (!CRoleSummaryMgr::Instance()->IsValidAcountRole(szOpenId, roleid))
	{
		roReply.m_nErrCode = IDIP_ERR_PLAYER_NOT_EXIST;
		roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
		return IDIP_SERVER_NONE;
	}
	SetRoleID(roleid);
	CRole* pRole = CRoleManager::Instance()->GetByRoleID(roleid);
	if (NULL == pRole)//不在线ms读取数据扣除;
	{
		return IDIP_SERVER_MS;
	}
	return IDIP_SERVER_GS;
}
bool CMSIdipUpdateScoreHandler::ModifyMsData(CUserData& roUserData)
{
	RoleAllInfo* pInfo = (RoleAllInfo*)roUserData.m_pUserPtr;
	CBag oData(pInfo);
	if (!oData.SetScore(m_uItemID, m_nNum))
	{
		return false;
	}
	oData.Save();
	return true;
}

void CMSIdipUpdateScoreHandler::IdipLog()
{
	rapidjson::Document doc;
	std::string s = GetStrJson();
	if (doc.Parse<0>(GetStrJson().c_str()).HasParseError())
	{
		return ;
	}
	TIdipUpdateScore oLog("TIdipUpdateScoreFlow");
	oLog.m_iScore = INT32(m_nNum);
	oLog.m_iScoreType = INT32(m_uItemID);
	oLog.m_strOpenId = szOpenId;
	oLog.m_uRoleId = GetRoleID();
	oLog.Do(doc["head"], doc["body"]);
}

bool CMSIdipUpdateScoreHandler::OnAsyncReplyFromGs(const CUserData& roUserData)
{
	if(roUserData.m_pUserPtr == NULL)
	{
		CRpc::ReplyDelayRpc(GetRpcID(), roUserData);
	}
	else
	{
		EIdipErrCode nErrCode = IDIP_ERR_UNKNOWN;
		if ((KKSG::ErrorCode)roUserData.m_dwUserData == ERR_SUCCESS)
		{
			CAccountKicker::Instance()->KickAccount(GetRoleID(), KKSG::KICK_NORMAL, NULL);
			nErrCode = IDIP_ERR_SUCCESS;
		}
		CRpc::ReplyDelayRpc(GetRpcID(), roUserData);
		IdipLog();
	}
	return true;
}

