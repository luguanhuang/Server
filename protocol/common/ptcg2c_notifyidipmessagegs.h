#ifndef __PTCG2C_NOTIFYIDIPMESSAGEGS_H__
#define __PTCG2C_NOTIFYIDIPMESSAGEGS_H__

// generate by ProtoGen at date: 2016/9/19 10:25:32

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_NOTIFYIDIPMESSAGEGS_TYPE 59353

class PtcG2C_NotifyIdipMessageGs : public CProtocol
{
public:
    explicit PtcG2C_NotifyIdipMessageGs() : CProtocol(PTCG2C_NOTIFYIDIPMESSAGEGS_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_NotifyIdipMessageGs()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::IdipMessage m_Data;
};

#endif