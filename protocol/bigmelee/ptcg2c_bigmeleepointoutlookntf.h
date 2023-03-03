#ifndef __PTCG2C_BIGMELEEPOINTOUTLOOKNTF_H__
#define __PTCG2C_BIGMELEEPOINTOUTLOOKNTF_H__

// generate by ProtoGen at date: 2017/10/10 15:40:02

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_BIGMELEEPOINTOUTLOOKNTF_TYPE 25027

class PtcG2C_BigMeleePointOutLookNtf : public CProtocol
{
public:
    explicit PtcG2C_BigMeleePointOutLookNtf() : CProtocol(PTCG2C_BIGMELEEPOINTOUTLOOKNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_BigMeleePointOutLookNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::BigMeleePointOutLook m_Data;
};

#endif