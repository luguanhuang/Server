#ifndef __PTCM2C_GOALAWARDSREDPOINT_H__
#define __PTCM2C_GOALAWARDSREDPOINT_H__

// generate by ProtoGen at date: 2017/9/1 19:48:09

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2C_GOALAWARDSREDPOINT_TYPE 11570

class PtcM2C_GoalAwardsRedPoint : public CProtocol
{
public:
    explicit PtcM2C_GoalAwardsRedPoint() : CProtocol(PTCM2C_GOALAWARDSREDPOINT_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2C_GoalAwardsRedPoint()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::GoalAwardsRedPointNtf m_Data;
};

#endif