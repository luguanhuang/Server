#ifndef __PTCG2C_STARTROLLNTF_H__
#define __PTCG2C_STARTROLLNTF_H__

// generate by ProtoGen at date: 2016/8/22 16:44:05

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_STARTROLLNTF_TYPE 41146

class PtcG2C_StartRollNtf : public CProtocol
{
public:
    explicit PtcG2C_StartRollNtf() : CProtocol(PTCG2C_STARTROLLNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_StartRollNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::StartRollNtfData m_Data;
};

#endif