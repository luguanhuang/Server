#ifndef __PTCG2C_GSERRORNOTIFY_H__
#define __PTCG2C_GSERRORNOTIFY_H__

// generate by ProtoGen at date: 2015/6/10 16:49:43

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_GSERRORNOTIFY_TYPE 2031

class PtcG2C_GSErrorNotify : public CProtocol
{
public:
    explicit PtcG2C_GSErrorNotify() : CProtocol(PTCG2C_GSERRORNOTIFY_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_GSErrorNotify()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::ErrorInfo m_Data;
};

#endif