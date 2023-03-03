#ifndef __PTCG2M_TEAMCOSTLEADERRESETNTF_H__
#define __PTCG2M_TEAMCOSTLEADERRESETNTF_H__

// generate by ProtoGen at date: 2017/5/17 22:23:54

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2M_TEAMCOSTLEADERRESETNTF_TYPE 11286

class PtcG2M_TeamCostLeaderResetNtf : public CProtocol
{
public:
    explicit PtcG2M_TeamCostLeaderResetNtf() : CProtocol(PTCG2M_TEAMCOSTLEADERRESETNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2M_TeamCostLeaderResetNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::TeamCostPara m_Data;
};

#endif