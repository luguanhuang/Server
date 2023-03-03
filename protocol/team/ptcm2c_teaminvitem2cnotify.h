#ifndef __PTCM2C_TEAMINVITEM2CNOTIFY_H__
#define __PTCM2C_TEAMINVITEM2CNOTIFY_H__

// generate by ProtoGen at date: 2016/8/18 14:15:06

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2C_TEAMINVITEM2CNOTIFY_TYPE 1221

class PtcM2C_TeamInviteM2CNotify : public CProtocol
{
public:
    explicit PtcM2C_TeamInviteM2CNotify() : CProtocol(PTCM2C_TEAMINVITEM2CNOTIFY_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2C_TeamInviteM2CNotify()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::TeamInvite m_Data;
};

#endif