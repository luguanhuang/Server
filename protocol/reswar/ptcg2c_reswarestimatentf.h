#ifndef __PTCG2C_RESWARESTIMATENTF_H__
#define __PTCG2C_RESWARESTIMATENTF_H__

// generate by ProtoGen at date: 2016/11/1 19:41:23

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_RESWARESTIMATENTF_TYPE 53855

class PtcG2C_ResWarEstimateNtf : public CProtocol
{
public:
    explicit PtcG2C_ResWarEstimateNtf() : CProtocol(PTCG2C_RESWARESTIMATENTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_ResWarEstimateNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::ResWarEstimateInfo m_Data;
};

#endif