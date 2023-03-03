#ifndef __PTCG2M_CHECKDRAGONGUILDTASK_H__
#define __PTCG2M_CHECKDRAGONGUILDTASK_H__

// generate by ProtoGen at date: 2017/9/12 14:39:21

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2M_CHECKDRAGONGUILDTASK_TYPE 49520

class PtcG2M_CheckDragonGuildTask : public CProtocol
{
public:
    explicit PtcG2M_CheckDragonGuildTask() : CProtocol(PTCG2M_CHECKDRAGONGUILDTASK_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2M_CheckDragonGuildTask()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::CheckDragonGuildTaskData m_Data;
};

#endif