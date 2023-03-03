#ifndef __PTCM2C_MSFLOWERRAINNTF_H__
#define __PTCM2C_MSFLOWERRAINNTF_H__

// generate by ProtoGen at date: 2016/8/18 17:28:57

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2C_MSFLOWERRAINNTF_TYPE 11986

class PtcM2C_MSFlowerRainNtf : public CProtocol
{
public:
    explicit PtcM2C_MSFlowerRainNtf() : CProtocol(PTCM2C_MSFLOWERRAINNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2C_MSFlowerRainNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::ReceiveFlowerData m_Data;
};

#endif