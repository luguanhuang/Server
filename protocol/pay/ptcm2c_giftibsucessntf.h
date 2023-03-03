#ifndef __PTCM2C_GIFTIBSUCESSNTF_H__
#define __PTCM2C_GIFTIBSUCESSNTF_H__

// generate by ProtoGen at date: 2017/5/4 18:01:28

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2C_GIFTIBSUCESSNTF_TYPE 29707

class PtcM2C_GiftIBSucessNtf : public CProtocol
{
public:
    explicit PtcM2C_GiftIBSucessNtf() : CProtocol(PTCM2C_GIFTIBSUCESSNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2C_GiftIBSucessNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::GiftIBBackInfo m_Data;
};

#endif