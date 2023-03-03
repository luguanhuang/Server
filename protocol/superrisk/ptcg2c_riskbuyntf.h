#ifndef __PTCG2C_RISKBUYNTF_H__
#define __PTCG2C_RISKBUYNTF_H__

// generate by ProtoGen at date: 2016/9/9 12:13:03

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_RISKBUYNTF_TYPE 61237

class PtcG2C_RiskBuyNtf : public CProtocol
{
public:
    explicit PtcG2C_RiskBuyNtf() : CProtocol(PTCG2C_RISKBUYNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_RiskBuyNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::RiskBuyData m_Data;
};

#endif