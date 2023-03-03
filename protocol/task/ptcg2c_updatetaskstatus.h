#ifndef __PTCG2C_UPDATETASKSTATUS_H__
#define __PTCG2C_UPDATETASKSTATUS_H__

// generate by ProtoGen at date: 2016/9/28 11:58:24

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_UPDATETASKSTATUS_TYPE 1609

class PtcG2C_UpdateTaskStatus : public CProtocol
{
public:
    explicit PtcG2C_UpdateTaskStatus() : CProtocol(PTCG2C_UPDATETASKSTATUS_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_UpdateTaskStatus()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::TaskInfo m_Data;
};

#endif