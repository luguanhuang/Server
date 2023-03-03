#ifndef __PTCM2G_CONSUMEREBATENTF_H__
#define __PTCM2G_CONSUMEREBATENTF_H__

// generate by ProtoGen at date: 2017/9/25 19:35:56

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2G_CONSUMEREBATENTF_TYPE 2016

class PtcM2G_ConsumeRebateNtf : public CProtocol
{
public:
    explicit PtcM2G_ConsumeRebateNtf() : CProtocol(PTCM2G_CONSUMEREBATENTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2G_ConsumeRebateNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::ConsumeRebateNtfData m_Data;
};

#endif