#ifndef __PTCG2M_GOALAWARDSSERVEREVENT_H__
#define __PTCG2M_GOALAWARDSSERVEREVENT_H__

// generate by ProtoGen at date: 2017/9/6 10:48:24

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2M_GOALAWARDSSERVEREVENT_TYPE 64621

class PtcG2M_GoalAwardsServerEvent : public CProtocol
{
public:
    explicit PtcG2M_GoalAwardsServerEvent() : CProtocol(PTCG2M_GOALAWARDSSERVEREVENT_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2M_GoalAwardsServerEvent()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::GoalAwardsServerEvent_G2M m_Data;
};

#endif