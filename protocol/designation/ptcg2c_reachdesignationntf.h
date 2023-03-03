#ifndef __PTCG2C_REACHDESIGNATIONNTF_H__
#define __PTCG2C_REACHDESIGNATIONNTF_H__

// generate by ProtoGen at date: 2016/6/2 15:51:57

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_REACHDESIGNATIONNTF_TYPE 17457

class PtcG2C_ReachDesignationNtf : public CProtocol
{
public:
    explicit PtcG2C_ReachDesignationNtf() : CProtocol(PTCG2C_REACHDESIGNATIONNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_ReachDesignationNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::ReachDesignationNtf m_Data;
};

#endif