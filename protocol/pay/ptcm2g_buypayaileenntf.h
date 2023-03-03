#ifndef __PTCM2G_BUYPAYAILEENNTF_H__
#define __PTCM2G_BUYPAYAILEENNTF_H__

// generate by ProtoGen at date: 2017/4/1 20:34:50

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2G_BUYPAYAILEENNTF_TYPE 45141

class PtcM2G_BuyPayAileenNtf : public CProtocol
{
public:
    explicit PtcM2G_BuyPayAileenNtf() : CProtocol(PTCM2G_BUYPAYAILEENNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2G_BuyPayAileenNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::BuyPayAileenStatus m_Data;
};

#endif