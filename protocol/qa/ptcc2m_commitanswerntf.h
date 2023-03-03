#ifndef __PTCC2M_COMMITANSWERNTF_H__
#define __PTCC2M_COMMITANSWERNTF_H__

// generate by ProtoGen at date: 2016/10/8 22:42:35

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCC2M_COMMITANSWERNTF_TYPE 12159

class PtcC2M_CommitAnswerNtf : public CProtocol
{
public:
    explicit PtcC2M_CommitAnswerNtf() : CProtocol(PTCC2M_COMMITANSWERNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcC2M_CommitAnswerNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::CommitAnswerNtf m_Data;
};

#endif