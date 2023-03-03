#ifndef __PTCM2C_ANSWERACKNTF_H__
#define __PTCM2C_ANSWERACKNTF_H__

// generate by ProtoGen at date: 2016/11/1 22:40:01

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2C_ANSWERACKNTF_TYPE 60141

class PtcM2C_AnswerAckNtf : public CProtocol
{
public:
    explicit PtcM2C_AnswerAckNtf() : CProtocol(PTCM2C_ANSWERACKNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2C_AnswerAckNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::AnswerAckNtf m_Data;
};

#endif