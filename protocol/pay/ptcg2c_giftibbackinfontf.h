#ifndef __PTCG2C_GIFTIBBACKINFONTF_H__
#define __PTCG2C_GIFTIBBACKINFONTF_H__

// generate by ProtoGen at date: 2017/3/14 14:09:51

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_GIFTIBBACKINFONTF_TYPE 6953

class PtcG2C_GiftIBBackInfoNtf : public CProtocol
{
public:
    explicit PtcG2C_GiftIBBackInfoNtf() : CProtocol(PTCG2C_GIFTIBBACKINFONTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_GiftIBBackInfoNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::GiftIBBackInfo m_Data;
};

#endif