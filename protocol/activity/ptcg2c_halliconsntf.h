#ifndef __PTCG2C_HALLICONSNTF_H__
#define __PTCG2C_HALLICONSNTF_H__

// generate by ProtoGen at date: 2016/9/24 17:54:07

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_HALLICONSNTF_TYPE 17871

class PtcG2C_HallIconSNtf : public CProtocol
{
public:
    explicit PtcG2C_HallIconSNtf() : CProtocol(PTCG2C_HALLICONSNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_HallIconSNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::HallIconPara m_Data;
};

#endif