#ifndef __PTCG2C_BIGMELEERELIVENTF_H__
#define __PTCG2C_BIGMELEERELIVENTF_H__

// generate by ProtoGen at date: 2017/6/22 8:16:46

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_BIGMELEERELIVENTF_TYPE 3358

class PtcG2C_BigMeleeReliveNtf : public CProtocol
{
public:
    explicit PtcG2C_BigMeleeReliveNtf() : CProtocol(PTCG2C_BIGMELEERELIVENTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_BigMeleeReliveNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::BigMeleeRelive m_Data;
};

#endif