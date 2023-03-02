#include "pch.h"
#include "ms_commondef_handler.h"
#include "util/jsonutil.h"
#include "account/punishmgr.h"

//������ʱMs��ȡRoleAllInfo�޸Ľ�������桢��ֹ�淨��punishmgr�޸Ľ������
//����ʱ gs�޸Ľ�������桢��ֹ�淨�ɹ���ms��punishmgr�޸�����;

IDIP_HANDLER_MODIFY_IMPLEMETION_EXCEPT_GSREPLAY(RelievePunish)
INT32 CMSIdipRelievePunishHandler::HandlerRequest(const rapidjson::Value& roHeadJson, const rapidjson::Value& roBodyJson, CIdipReply& roReply)
{
	UINT64 roleid = JsonUtil::GetJsonUint64(roBodyJson, "RoleId");
	strOpenId = JsonUtil::GetJsonString(roBodyJson, "OpenId");
	if (!CRoleSummaryMgr::Instance()->IsValidAcountRole(strOpenId, roleid))
	{
		roReply.m_nErrCode = IDIP_ERR_PLAYER_NOT_EXIST;
		roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
		return IDIP_SERVER_NONE;
	}
	SetRoleID(roleid);
	m_bIsRelieveZeroProfit = JsonUtil::GetJsonUint(roBodyJson, "RelieveZeroProfit")==1?true:false;
	m_bIsRelievePlayAll = JsonUtil::GetJsonUint(roBodyJson, "RelievePlayAll")==1?true:false;
	m_bIsRelieveBanJoinRank = JsonUtil::GetJsonUint(roBodyJson, "RelieveBanJoinRank")==1?true:false;
	m_bIsRelieveBan = JsonUtil::GetJsonUint(roBodyJson, "RelieveBan")==1?true:false;
	m_bIsRelieveMaskchat = JsonUtil::GetJsonUint(roBodyJson, "RelieveMaskchat")==1?true:false;
	
	CRole* pRole = CRoleManager::Instance()->GetByRoleID(roleid);
	if (NULL == pRole)//������ms��ȡ���ݿ۳�;
	{
		return IDIP_SERVER_MS;
	}
	return IDIP_SERVER_GS;
}
bool CMSIdipRelievePunishHandler::ModifyMsData(CUserData& roUserData)
{
	RoleAllInfo* pInfo = (RoleAllInfo*)roUserData.m_pUserPtr;
	KKSG::IdipData roInfo;
	roInfo.CopyFrom(pInfo->idiprecord());
	pInfo->mutable_idiprecord()->Clear();
	pInfo->mutable_idiprecord()->mutable_mess()->set_message(roInfo.mess().message());
	for (INT32 i = 0; i < roInfo.punishinfo_size(); ++ i)
	{
		if (m_bIsRelieveZeroProfit && roInfo.punishinfo(i).type() == KKSG::PUNISH_USER_ZERO_PROFIT)
		{
			continue;
		}
		if (m_bIsRelievePlayAll && (roInfo.punishinfo(i).type() == KKSG::PUNISH_USER_DAILY_PLAY || roInfo.punishinfo(i).type() == KKSG::PUNISH_USER_MULTI_ACTIVITY))
		{
			continue;
		}
		KKSG::IdipPunishData* pData = pInfo->mutable_idiprecord()->add_punishinfo();
		pData->CopyFrom(roInfo.punishinfo(i));
	}
	UnPunishMs();
	return true;
}
void CMSIdipRelievePunishHandler::UnPunishMs()
{
	if (m_bIsRelieveBanJoinRank)
	{
		CPunishMgr::Instance()->UnPunishALLRank(GetRoleID());
	}

	if (m_bIsRelieveBan)
	{
		CPunishMgr::Instance()->UnDoPunishUser(GetRoleID(), KKSG::PUNISH_USER_LOGIN);
	}

	if (m_bIsRelieveMaskchat)
	{
		CPunishMgr::Instance()->UnDoPunishUser(GetRoleID(), KKSG::PUNISH_USER_CHAT);
	}

	if (m_bIsRelievePlayAll)
	{
		CRole* pRole = CRoleManager::Instance()->GetByRoleID(GetRoleID());
		if (pRole)
		{
			pRole->UnDoIdipPunishData(KKSG::PUNISH_USER_DAILY_PLAY);
			pRole->UnDoIdipPunishData(KKSG::PUNISH_USER_MULTI_ACTIVITY);
		}
	}
}
bool CMSIdipRelievePunishHandler::OnAsyncReplyFromGs(const CUserData& roUserData)
{
	if (roUserData.m_pUserPtr != NULL)
	{
		if ((KKSG::ErrorCode)roUserData.m_dwUserData == ERR_SUCCESS)
		{
			UnPunishMs();
			IdipLog();
		}
	}
	CRpc::ReplyDelayRpc(GetRpcID(), roUserData);
	return true;
}
void CMSIdipRelievePunishHandler::IdipLog()
{
	rapidjson::Document doc;
	std::string s = GetStrJson();
	if (doc.Parse<0>(GetStrJson().c_str()).HasParseError())
	{
		return ;
	}
	TIdipRelievePunish oLog("TIdipAqDoRelievePunishFlow");
	oLog.m_uRelieveZeroProfit = m_bIsRelieveZeroProfit?1:0;
	oLog.m_uRelievePlayAll = m_bIsRelievePlayAll?1:0;
	oLog.m_uRelieveBanJoinRank = m_bIsRelieveBanJoinRank?1:0;
	oLog.m_uRelieveBan = m_bIsRelieveBan?1:0;
	oLog.m_uRelieveMaskchat = m_bIsRelieveMaskchat?1:0;
	oLog.m_uRoleId = GetRoleID();
	oLog.m_strOpenId = strOpenId;
	oLog.Do(doc["head"], doc["body"]);
}
