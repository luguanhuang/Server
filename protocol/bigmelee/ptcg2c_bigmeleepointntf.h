#ifndef __PTCG2C_BIGMELEEPOINTNTF_H__
#define __PTCG2C_BIGMELEEPOINTNTF_H__

// generate by ProtoGen at date: 2017/10/10 15:04:06

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_BIGMELEEPOINTNTF_TYPE 15624

class PtcG2C_BigMeleePointNtf : public CProtocol
{
public:
    explicit PtcG2C_BigMeleePointNtf() : CProtocol(PTCG2C_BIGMELEEPOINTNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_BigMeleePointNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::BigMeleePoint m_Data;
};

#endif