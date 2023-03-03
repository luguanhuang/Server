#ifndef __PTCM2C_PUSHQUESTIONNTF_H__
#define __PTCM2C_PUSHQUESTIONNTF_H__

// generate by ProtoGen at date: 2016/10/9 11:18:43

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2C_PUSHQUESTIONNTF_TYPE 45138

class PtcM2C_PushQuestionNtf : public CProtocol
{
public:
    explicit PtcM2C_PushQuestionNtf() : CProtocol(PTCM2C_PUSHQUESTIONNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2C_PushQuestionNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::PushQuestionNtf m_Data;
};

#endif