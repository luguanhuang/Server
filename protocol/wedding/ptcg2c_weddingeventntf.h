#ifndef __PTCG2C_WEDDINGEVENTNTF_H__
#define __PTCG2C_WEDDINGEVENTNTF_H__

// generate by ProtoGen at date: 2017/7/26 16:41:58

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_WEDDINGEVENTNTF_TYPE 51472

class PtcG2C_WeddingEventNtf : public CProtocol
{
public:
    explicit PtcG2C_WeddingEventNtf() : CProtocol(PTCG2C_WEDDINGEVENTNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_WeddingEventNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::WeddingEventNtf m_Data;
};

#endif