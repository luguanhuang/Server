#ifndef __PTCG2C_RECEIVEFLOWERNTF_H__
#define __PTCG2C_RECEIVEFLOWERNTF_H__

// generate by ProtoGen at date: 2016/7/8 20:33:56

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_RECEIVEFLOWERNTF_TYPE 43606

class PtcG2C_ReceiveFlowerNtf : public CProtocol
{
public:
    explicit PtcG2C_ReceiveFlowerNtf() : CProtocol(PTCG2C_RECEIVEFLOWERNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_ReceiveFlowerNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::ReceiveFlowerData m_Data;
};

#endif