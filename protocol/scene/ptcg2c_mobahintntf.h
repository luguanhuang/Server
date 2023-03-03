#ifndef __PTCG2C_MOBAHINTNTF_H__
#define __PTCG2C_MOBAHINTNTF_H__

// generate by ProtoGen at date: 2017/6/23 10:27:14

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_MOBAHINTNTF_TYPE 17027

class PtcG2C_MobaHintNtf : public CProtocol
{
public:
    explicit PtcG2C_MobaHintNtf() : CProtocol(PTCG2C_MOBAHINTNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_MobaHintNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::MobaHintNtf m_Data;
};

#endif