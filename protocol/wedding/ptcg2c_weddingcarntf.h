#ifndef __PTCG2C_WEDDINGCARNTF_H__
#define __PTCG2C_WEDDINGCARNTF_H__

// generate by ProtoGen at date: 2017/8/3 17:01:06

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_WEDDINGCARNTF_TYPE 48301

class PtcG2C_WeddingCarNtf : public CProtocol
{
public:
    explicit PtcG2C_WeddingCarNtf() : CProtocol(PTCG2C_WEDDINGCARNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_WeddingCarNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::WeddingCarNotify m_Data;
};

#endif