#ifndef __PTCG2T_OPENGATE_H__
#define __PTCG2T_OPENGATE_H__

// generate by ProtoGen at date: 2016/12/25 18:53:05

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2T_OPENGATE_TYPE 54510

class PtcG2T_OpenGate : public CProtocol
{
public:
    explicit PtcG2T_OpenGate() : CProtocol(PTCG2T_OPENGATE_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2T_OpenGate()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::gateflag m_Data;
};

#endif