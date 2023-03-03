#ifndef __PTCG2C_SYNCMOVENOTIFY_H__
#define __PTCG2C_SYNCMOVENOTIFY_H__

// generate by ProtoGen at date: 2016/11/28 16:17:18

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_SYNCMOVENOTIFY_TYPE 32838

class PtcG2C_SyncMoveNotify : public CProtocol
{
public:
    explicit PtcG2C_SyncMoveNotify() : CProtocol(PTCG2C_SYNCMOVENOTIFY_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_SyncMoveNotify()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::StepMoveData m_Data;
};

#endif