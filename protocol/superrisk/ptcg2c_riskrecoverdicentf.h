#ifndef __PTCG2C_RISKRECOVERDICENTF_H__
#define __PTCG2C_RISKRECOVERDICENTF_H__

// generate by ProtoGen at date: 2016/9/9 20:46:29

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_RISKRECOVERDICENTF_TYPE 45917

class PtcG2C_RiskRecoverDiceNtf : public CProtocol
{
public:
    explicit PtcG2C_RiskRecoverDiceNtf() : CProtocol(PTCG2C_RISKRECOVERDICENTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_RiskRecoverDiceNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::RiskRecoverDiceData m_Data;
};

#endif