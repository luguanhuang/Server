#ifndef __PTCM2C_MSRECEIVEFLOWERNTF_H__
#define __PTCM2C_MSRECEIVEFLOWERNTF_H__

// generate by ProtoGen at date: 2016/8/18 17:15:42

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2C_MSRECEIVEFLOWERNTF_TYPE 16969

class PtcM2C_MSReceiveFlowerNtf : public CProtocol
{
public:
    explicit PtcM2C_MSReceiveFlowerNtf() : CProtocol(PTCM2C_MSRECEIVEFLOWERNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2C_MSReceiveFlowerNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::ReceiveFlowerData m_Data;
};

#endif