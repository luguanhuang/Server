#ifndef __PTCG2C_REVIVECOUNTDOWN_H__
#define __PTCG2C_REVIVECOUNTDOWN_H__

// generate by ProtoGen at date: 2015/7/22 20:28:41

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_REVIVECOUNTDOWN_TYPE 54507

class PtcG2C_ReviveCountdown : public CProtocol
{
public:
    explicit PtcG2C_ReviveCountdown() : CProtocol(PTCG2C_REVIVECOUNTDOWN_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_ReviveCountdown()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::ReviveCountdownInfo m_Data;
};

#endif