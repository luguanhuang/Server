#ifndef __PTCG2C_COUNTDOWNNTF_H__
#define __PTCG2C_COUNTDOWNNTF_H__

// generate by ProtoGen at date: 2017/3/7 16:06:06

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_COUNTDOWNNTF_TYPE 3259

class PtcG2C_CountDownNtf : public CProtocol
{
public:
    explicit PtcG2C_CountDownNtf() : CProtocol(PTCG2C_COUNTDOWNNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_CountDownNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::CountDownNtf m_Data;
};

#endif