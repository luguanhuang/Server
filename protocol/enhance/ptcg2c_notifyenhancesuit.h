#ifndef __PTCG2C_NOTIFYENHANCESUIT_H__
#define __PTCG2C_NOTIFYENHANCESUIT_H__

// generate by ProtoGen at date: 2016/9/7 22:28:32

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_NOTIFYENHANCESUIT_TYPE 44091

class PtcG2C_NotifyEnhanceSuit : public CProtocol
{
public:
    explicit PtcG2C_NotifyEnhanceSuit() : CProtocol(PTCG2C_NOTIFYENHANCESUIT_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_NotifyEnhanceSuit()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::NotifyEnhanceSuit m_Data;
};

#endif