#ifndef __PTCG2C_REACHACHIEVENTF_H__
#define __PTCG2C_REACHACHIEVENTF_H__

// generate by ProtoGen at date: 2016/6/2 15:51:54

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_REACHACHIEVENTF_TYPE 1479

class PtcG2C_ReachAchieveNtf : public CProtocol
{
public:
    explicit PtcG2C_ReachAchieveNtf() : CProtocol(PTCG2C_REACHACHIEVENTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_ReachAchieveNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::ReachAchieveNtf m_Data;
};

#endif