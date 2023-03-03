#ifndef __PTCG2C_FLOWERRAINNTF_H__
#define __PTCG2C_FLOWERRAINNTF_H__

// generate by ProtoGen at date: 2016/7/8 20:35:51

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_FLOWERRAINNTF_TYPE 30604

class PtcG2C_FlowerRainNtf : public CProtocol
{
public:
    explicit PtcG2C_FlowerRainNtf() : CProtocol(PTCG2C_FLOWERRAINNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_FlowerRainNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::ReceiveFlowerData m_Data;
};

#endif