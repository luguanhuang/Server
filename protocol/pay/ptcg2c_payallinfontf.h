#ifndef __PTCG2C_PAYALLINFONTF_H__
#define __PTCG2C_PAYALLINFONTF_H__

// generate by ProtoGen at date: 2016/8/11 19:57:20

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_PAYALLINFONTF_TYPE 4976

class PtcG2C_PayAllInfoNtf : public CProtocol
{
public:
    explicit PtcG2C_PayAllInfoNtf() : CProtocol(PTCG2C_PAYALLINFONTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_PayAllInfoNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::PayAllInfo m_Data;
};

#endif