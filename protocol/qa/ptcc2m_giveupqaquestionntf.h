#ifndef __PTCC2M_GIVEUPQAQUESTIONNTF_H__
#define __PTCC2M_GIVEUPQAQUESTIONNTF_H__

// generate by ProtoGen at date: 2016/10/8 23:11:39

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCC2M_GIVEUPQAQUESTIONNTF_TYPE 17022

class PtcC2M_GiveUpQAQuestionNtf : public CProtocol
{
public:
    explicit PtcC2M_GiveUpQAQuestionNtf() : CProtocol(PTCC2M_GIVEUPQAQUESTIONNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcC2M_GiveUpQAQuestionNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::GiveUpQuestionNtf m_Data;
};

#endif