#ifndef __PTCG2C_WEDDINGSTATENTF_H__
#define __PTCG2C_WEDDINGSTATENTF_H__

// generate by ProtoGen at date: 2017/7/26 16:29:25

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_WEDDINGSTATENTF_TYPE 30976

class PtcG2C_WeddingStateNtf : public CProtocol
{
public:
    explicit PtcG2C_WeddingStateNtf() : CProtocol(PTCG2C_WEDDINGSTATENTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_WeddingStateNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::WeddingStateNtf m_Data;
};

#endif