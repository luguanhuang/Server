#ifndef __PTCG2M_UPDATEROLESUMMARY_H__
#define __PTCG2M_UPDATEROLESUMMARY_H__

// generate by ProtoGen at date: 2016/7/19 9:37:09

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2M_UPDATEROLESUMMARY_TYPE 8458

class PtcG2M_UpdateRoleSummary : public CProtocol
{
public:
    explicit PtcG2M_UpdateRoleSummary() : CProtocol(PTCG2M_UPDATEROLESUMMARY_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2M_UpdateRoleSummary()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::UpdateRoleSummaryData m_Data;
};

#endif