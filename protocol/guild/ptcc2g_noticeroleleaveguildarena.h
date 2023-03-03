#ifndef __PTCC2G_NOTICEROLELEAVEGUILDARENA_H__
#define __PTCC2G_NOTICEROLELEAVEGUILDARENA_H__

// generate by ProtoGen at date: 2016/7/19 19:54:15

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCC2G_NOTICEROLELEAVEGUILDARENA_TYPE 24969

class PtcC2G_NoticeRoleLeaveGuildArena : public CProtocol
{
public:
    explicit PtcC2G_NoticeRoleLeaveGuildArena() : CProtocol(PTCC2G_NOTICEROLELEAVEGUILDARENA_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcC2G_NoticeRoleLeaveGuildArena()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::NoticeRoleLeaveGuildArena m_Data;
};

#endif