#ifndef __PTCG2C_HEROBATTLETIPSNTF_H__
#define __PTCG2C_HEROBATTLETIPSNTF_H__

// generate by ProtoGen at date: 2017/1/11 11:46:58

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_HEROBATTLETIPSNTF_TYPE 15389

class PtcG2C_HeroBattleTipsNtf : public CProtocol
{
public:
    explicit PtcG2C_HeroBattleTipsNtf() : CProtocol(PTCG2C_HEROBATTLETIPSNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_HeroBattleTipsNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::HeroBattleTipsData m_Data;
};

#endif