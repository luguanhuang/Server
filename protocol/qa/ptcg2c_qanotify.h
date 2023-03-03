#ifndef __PTCG2C_QANOTIFY_H__
#define __PTCG2C_QANOTIFY_H__

// generate by ProtoGen at date: 2016/6/23 19:37:20

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_QANOTIFY_TYPE 37337

class PtcG2C_QANotify : public CProtocol
{
public:
    explicit PtcG2C_QANotify() : CProtocol(PTCG2C_QANOTIFY_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_QANotify()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::QANotify m_Data;
};

#endif