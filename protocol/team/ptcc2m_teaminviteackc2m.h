#ifndef __PTCC2M_TEAMINVITEACKC2M_H__
#define __PTCC2M_TEAMINVITEACKC2M_H__

// generate by ProtoGen at date: 2016/8/18 14:23:53

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCC2M_TEAMINVITEACKC2M_TYPE 15365

class PtcC2M_TeamInviteAckC2M : public CProtocol
{
public:
    explicit PtcC2M_TeamInviteAckC2M() : CProtocol(PTCC2M_TEAMINVITEACKC2M_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcC2M_TeamInviteAckC2M()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::TeamInviteAck m_Data;
};

#endif