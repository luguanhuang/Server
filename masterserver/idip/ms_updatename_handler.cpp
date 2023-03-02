#include "pch.h"
#include "ms_commondef_handler.h"
#include "util/jsonutil.h"
#include "friend/friend.h"
#include "chat/ptcm2t_chatforward.h"
#include "friend/friendop.h"
#include "define/noticedef.h"
#include "chat/chatmgr.h"
#include "notice/notice.h"
#include "notice/noticemgr.h"
#include "account/accountkick.h"
#include "LogInit.h"

IDIP_HANDLER_READANDSAVE_IMPLEMETION(UpdateName)
INT32 CMSIdipUpdateNameHandler::HandlerRequest(const rapidjson::Value& roHeadJson, const rapidjson::Value& roBodyJson, CIdipReply& roReply)
{
	UINT64 roleid = JsonUtil::GetJsonUint64(roBodyJson, "RoleId");
	szOpenId = JsonUtil::GetJsonString(roBodyJson, "OpenId");
	szName = JsonUtil::GetJsonString(roBodyJson, "Name");
	szMailTitle = JsonUtil::GetJsonString(roBodyJson, "MailTitle");
	szMailContent = JsonUtil::GetJsonString(roBodyJson, "MailContent");
	if (!CRoleSummaryMgr::Instance()->IsValidAcountRole(szOpenId, roleid))
	{
		roReply.m_nErrCode = IDIP_ERR_PLAYER_NOT_EXIST;
		roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
		return IDIP_SERVER_NONE;
	}
	SetRoleID(roleid);

	if (CRoleSummaryMgr::Instance()->IsNameInvalid(szName) != KKSG::ERR_SUCCESS)
	{
		roReply.m_nErrCode = IDIP_ERR_INVALID_PARA;
		roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
		return IDIP_SERVER_NONE;
	}

	LogInfo("openid=%s,roleid=%llu idip change name to %s",szOpenId.c_str(), roleid, szName.c_str());
	CRoleSummaryMgr::Instance()->LockName(szName);
	CRole* pRole = CRoleManager::Instance()->GetByRoleID(roleid);
	if (NULL == pRole)
	{
		return IDIP_SERVER_MS;
	}
	return IDIP_SERVER_GS;
}

bool CMSIdipUpdateNameHandler::ModifyMsData(CUserData& roUserData)
{
	RoleAllInfo* pInfo = (RoleAllInfo*)roUserData.m_pUserPtr;
	pInfo->mutable_brief()->set_name(szName);
	return true;
}

void CMSIdipUpdateNameHandler::DoAfterChangeName()
{
	CRole* role = CRoleManager::Instance()->GetByRoleID(GetRoleID());
	if (NULL != role)
	{
		role->SetName(szName);
	}
	CRoleSummary* summary = CRoleSummaryMgr::Instance()->GetRoleSummary(GetRoleID());	
	if (NULL != summary)
	{
		std::string lastname = summary->GetName();
		CRoleSummaryMgr::Instance()->ChangeName(lastname, szName);
		summary->SetName(szName);

		// notice friend
		FriendOp op(summary->GetID());
		const std::map<UINT64, FriendInfo>* friendlist = op.GetFriendList();
		if (NULL != friendlist)
		{
			for (auto i = friendlist->begin(); i != friendlist->end(); ++i)
			{
				PtcM2T_chatforward ptc;
				KKSG::ChatInfo* chatinfo = ptc.m_Data.mutable_chatinfo();
				// channel
				chatinfo->set_channel(KKSG::PrivateChannel);
				// source
				chatinfo->mutable_source()->set_roleid(summary->GetID());
				chatinfo->set_time(TimeUtil::GetTime());
				// dest
				KKSG::ChatSource* dest = chatinfo->add_destlist();
				dest->set_roleid(i->first);
				// info
				Notice notice(ChangeNameNotice1);
				notice.Replace("$C", lastname);
				notice.Replace("$C", szName);
				chatinfo->set_info(notice.chatinfo->info());

				CChatMgr::Instance()->AddChat(summary->GetID(), *chatinfo);
				CChatMgr::Instance()->AddChat(i->first, *chatinfo);

				///> 向用户发送聊天消息
				CChatMgr::Instance()->SendUser(summary->GetID(), *chatinfo, true);
				CChatMgr::Instance()->SendUser(i->first, *chatinfo, true);
			}
		}

		// notice guild
		UINT64 guildid = CGuildMgr::Instance()->GetGuildIdByRoleId(summary->GetID());
		if (INVALID_GUILDID != guildid)
		{
			NoticeMgr::Instance()->NoticeNameChange(guildid, lastname, szName);
		}
	}
	std::vector<ItemDesc> items;
	MailData oData(szMailTitle, szMailContent, items, MAIL_EXPIRE_TIME);
	CMailMgr::Instance()->SendMail(GetRoleID(), oData);
}

bool CMSIdipUpdateNameHandler::OnAsyncReplyFromGs(const CUserData& roUserData)
{
	CRoleSummaryMgr::Instance()->UnlockName(szName);
	if (roUserData.m_pUserPtr != NULL && roUserData.m_dwUserData == KKSG::ERR_SUCCESS)
	{
		DoAfterChangeName();
		CAccountKicker::Instance()->KickAccount(GetRoleID(), KKSG::KICK_NORMAL, NULL);
	}
	CRpc::ReplyDelayRpc(GetRpcID(), roUserData);
	return true;
}

bool CMSIdipUpdateNameHandler::OnAsyncReply(const CUserData& roUserData)
{
	CRoleSummaryMgr::Instance()->UnlockName(szName);
	if(roUserData.m_pUserPtr == NULL)
	{
		//timeout
		CRpc::ReplyDelayRpc(GetRpcID(), roUserData);
	}
	else
	{		
		DoAfterChangeName();
		std::string str;
		CIdipReply oReply(str);
		oReply.m_nErrCode = IDIP_ERR_SUCCESS;
		oReply.m_strResult = PackErrResponse(oReply.m_nErrCode);

		CUserData data;
		data.m_pUserPtr = &oReply;
		CRpc::ReplyDelayRpc(GetRpcID(), data);
	}
	return true;
}
