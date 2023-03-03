#ifndef __PTCG2C_TEAMINVITENOTIFY_H__
#define __PTCG2C_TEAMINVITENOTIFY_H__

// generate by ProtoGen at date: 2015/11/30 19:32:41

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_TEAMINVITENOTIFY_TYPE 4060

class PtcG2C_TeamInviteNotify : public CProtocol
{
public:
    explicit PtcG2C_TeamInviteNotify() : CProtocol(PTCG2C_TEAMINVITENOTIFY_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_TeamInviteNotify()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::TeamInvite m_Data;
};

#endif