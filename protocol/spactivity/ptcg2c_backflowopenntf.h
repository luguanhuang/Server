#ifndef __PTCG2C_BACKFLOWOPENNTF_H__
#define __PTCG2C_BACKFLOWOPENNTF_H__

// generate by ProtoGen at date: 2017/10/21 16:38:38

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_BACKFLOWOPENNTF_TYPE 27749

class PtcG2C_BackFlowOpenNtf : public CProtocol
{
public:
    explicit PtcG2C_BackFlowOpenNtf() : CProtocol(PTCG2C_BACKFLOWOPENNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_BackFlowOpenNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::BackFlowOpenNtf m_Data;
};

#endif