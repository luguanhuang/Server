#ifndef __PTCG2C_MOBAADDEXPNTF_H__
#define __PTCG2C_MOBAADDEXPNTF_H__

// generate by ProtoGen at date: 2017/6/21 21:27:07

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_MOBAADDEXPNTF_TYPE 36674

class PtcG2C_MobaAddExpNtf : public CProtocol
{
public:
    explicit PtcG2C_MobaAddExpNtf() : CProtocol(PTCG2C_MOBAADDEXPNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_MobaAddExpNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::MobaAddExpData m_Data;
};

#endif