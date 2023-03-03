#ifndef __PTCG2C_TAKERANDOMTASK_H__
#define __PTCG2C_TAKERANDOMTASK_H__

// generate by ProtoGen at date: 2015/11/26 21:23:46

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_TAKERANDOMTASK_TYPE 8442

class PtcG2C_TakeRandomTask : public CProtocol
{
public:
    explicit PtcG2C_TakeRandomTask() : CProtocol(PTCG2C_TAKERANDOMTASK_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_TakeRandomTask()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::randomtask m_Data;
};

#endif