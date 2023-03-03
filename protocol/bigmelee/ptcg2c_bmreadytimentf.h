#ifndef __PTCG2C_BMREADYTIMENTF_H__
#define __PTCG2C_BMREADYTIMENTF_H__

// generate by ProtoGen at date: 2017/6/22 8:19:01

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_BMREADYTIMENTF_TYPE 8612

class PtcG2C_BMReadyTimeNtf : public CProtocol
{
public:
    explicit PtcG2C_BMReadyTimeNtf() : CProtocol(PTCG2C_BMREADYTIMENTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_BMReadyTimeNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::BMReadyTime m_Data;
};

#endif