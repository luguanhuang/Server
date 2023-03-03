#ifndef __PTCG2C_PAYSCORENTF_H__
#define __PTCG2C_PAYSCORENTF_H__

// generate by ProtoGen at date: 2017/9/7 15:47:16

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_PAYSCORENTF_TYPE 61859

class PtcG2C_PayScoreNtf : public CProtocol
{
public:
    explicit PtcG2C_PayScoreNtf() : CProtocol(PTCG2C_PAYSCORENTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_PayScoreNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::PayScoreData m_Data;
};

#endif