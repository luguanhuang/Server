#ifndef __PTCG2M_ADDPARTNERLIVENESSG2M_H__
#define __PTCG2M_ADDPARTNERLIVENESSG2M_H__

// generate by ProtoGen at date: 2017/3/15 10:56:26

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2M_ADDPARTNERLIVENESSG2M_TYPE 56610

class PtcG2M_AddPartnerLivenessG2M : public CProtocol
{
public:
    explicit PtcG2M_AddPartnerLivenessG2M() : CProtocol(PTCG2M_ADDPARTNERLIVENESSG2M_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2M_AddPartnerLivenessG2M()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::AddPartnerLiveness m_Data;
};

#endif