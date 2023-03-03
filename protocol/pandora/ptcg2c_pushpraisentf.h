#ifndef __PTCG2C_PUSHPRAISENTF_H__
#define __PTCG2C_PUSHPRAISENTF_H__

// generate by ProtoGen at date: 2017/1/9 22:40:20

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_PUSHPRAISENTF_TYPE 5686

class PtcG2C_PushPraiseNtf : public CProtocol
{
public:
    explicit PtcG2C_PushPraiseNtf() : CProtocol(PTCG2C_PUSHPRAISENTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_PushPraiseNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::PushPraise m_Data;
};

#endif