#ifndef __PTCM2G_GOALAWARDSSERVERAWARDS_H__
#define __PTCM2G_GOALAWARDSSERVERAWARDS_H__

// generate by ProtoGen at date: 2017/9/6 10:47:09

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2G_GOALAWARDSSERVERAWARDS_TYPE 32122

class PtcM2G_GoalAwardsServerAwards : public CProtocol
{
public:
    explicit PtcM2G_GoalAwardsServerAwards() : CProtocol(PTCM2G_GOALAWARDSSERVERAWARDS_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2G_GoalAwardsServerAwards()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::GoalAwardsServerAwards_M2G m_Data;
};

#endif