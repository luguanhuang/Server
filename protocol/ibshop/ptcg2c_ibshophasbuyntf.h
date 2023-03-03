#ifndef __PTCG2C_IBSHOPHASBUYNTF_H__
#define __PTCG2C_IBSHOPHASBUYNTF_H__

// generate by ProtoGen at date: 2016/12/30 13:35:50

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_IBSHOPHASBUYNTF_TYPE 12835

class PtcG2C_IBShopHasBuyNtf : public CProtocol
{
public:
    explicit PtcG2C_IBShopHasBuyNtf() : CProtocol(PTCG2C_IBSHOPHASBUYNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_IBShopHasBuyNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::IBShopHasBuy m_Data;
};

#endif