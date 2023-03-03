#ifndef __PTCM2C_NOTIFYGUILDCHECKINPROCESS_H__
#define __PTCM2C_NOTIFYGUILDCHECKINPROCESS_H__

// generate by ProtoGen at date: 2016/9/18 16:01:13

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2C_NOTIFYGUILDCHECKINPROCESS_TYPE 34704

class PtcM2C_NotifyGuildCheckinProcess : public CProtocol
{
public:
    explicit PtcM2C_NotifyGuildCheckinProcess() : CProtocol(PTCM2C_NOTIFYGUILDCHECKINPROCESS_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2C_NotifyGuildCheckinProcess()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::GuildCheckinBoxNtf m_Data;
};

#endif