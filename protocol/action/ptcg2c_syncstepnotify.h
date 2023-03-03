#ifndef __PTCG2C_SYNCSTEPNOTIFY_H__
#define __PTCG2C_SYNCSTEPNOTIFY_H__

// generate by ProtoGen at date: 2016/6/16 14:25:10

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_SYNCSTEPNOTIFY_TYPE 37999

class PtcG2C_SyncStepNotify : public CProtocol
{
public:
    explicit PtcG2C_SyncStepNotify() : CProtocol(PTCG2C_SYNCSTEPNOTIFY_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_SyncStepNotify()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::StepSyncInfo m_Data;
};

#endif