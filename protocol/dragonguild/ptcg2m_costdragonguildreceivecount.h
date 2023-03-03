#ifndef __PTCG2M_COSTDRAGONGUILDRECEIVECOUNT_H__
#define __PTCG2M_COSTDRAGONGUILDRECEIVECOUNT_H__

// generate by ProtoGen at date: 2017/9/12 14:28:34

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2M_COSTDRAGONGUILDRECEIVECOUNT_TYPE 47101

class PtcG2M_CostDragonGuildReceiveCount : public CProtocol
{
public:
    explicit PtcG2M_CostDragonGuildReceiveCount() : CProtocol(PTCG2M_COSTDRAGONGUILDRECEIVECOUNT_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2M_CostDragonGuildReceiveCount()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::CostDragonGuildReceiveCountData m_Data;
};

#endif