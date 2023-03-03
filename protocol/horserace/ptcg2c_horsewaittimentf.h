#ifndef __PTCG2C_HORSEWAITTIMENTF_H__
#define __PTCG2C_HORSEWAITTIMENTF_H__

// generate by ProtoGen at date: 2016/12/6 20:52:41

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_HORSEWAITTIMENTF_TYPE 34138

class PtcG2C_HorseWaitTimeNtf : public CProtocol
{
public:
    explicit PtcG2C_HorseWaitTimeNtf() : CProtocol(PTCG2C_HORSEWAITTIMENTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_HorseWaitTimeNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::HorseWaitTime m_Data;
};

#endif