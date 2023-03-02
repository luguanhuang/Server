#include "pch.h"
#include "chat/ptcg2m_noticeforwardntf.h"
#include "notice/notice.h"
#include "guild/guildmgr.h"

// generate by ProtoGen at date: 2016/7/26 22:46:49

void PtcG2M_NoticeForwardNtf::Process(UINT32 dwConnID)
{
	if (m_Data.type() == KKSG::Notice_Guild && m_Data.uid() == 0)
	{
		UINT64 guildid = CGuildMgr::Instance()->GetGuildIdByRoleId(m_Data.roleid());
		m_Data.set_uid(guildid);
	}
	Notice notice(m_Data);
	notice.Send();
}
