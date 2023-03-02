#include "pch.h"
#include "timeutil.h"
#include "define/noticedef.h"
#include "chat/chatmgr.h"
#include "chat/ptcm2t_chatforward.h"
#include "chat/ptcg2m_privatechat.h"

// generate by ProtoGen at date: 2017/9/15 15:00:28

void PtcG2M_PrivateChat::Process(UINT32 dwConnID)
{
	UINT64 srcroleid = m_Data.sourceroleid();
	UINT64 destroleid = m_Data.destroleid();
	std::string content = m_Data.content();

	PtcM2T_chatforward ptc;
	KKSG::ChatInfo* chatinfo = ptc.m_Data.mutable_chatinfo();
	// channel
	chatinfo->set_channel(KKSG::PrivateChannel);
	// source
	chatinfo->mutable_source()->set_roleid(srcroleid);
	chatinfo->set_time(TimeUtil::GetTime());
	// dest
	KKSG::ChatSource* dest = chatinfo->add_destlist();
	dest->set_roleid(destroleid);
	// info
	chatinfo->set_info(content.c_str());

	CChatMgr::Instance()->AddChat(srcroleid, *chatinfo);
	CChatMgr::Instance()->AddChat(destroleid, *chatinfo);

	///> 向用户发送聊天消息
	CChatMgr::Instance()->SendUser(srcroleid, *chatinfo, true);
	CChatMgr::Instance()->SendUser(destroleid, *chatinfo, true);

}
