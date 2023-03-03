#ifndef __PTCC2M_PAYBUYGOODSFAILNTF_H__
#define __PTCC2M_PAYBUYGOODSFAILNTF_H__

// generate by ProtoGen at date: 2017/3/16 22:45:50

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCC2M_PAYBUYGOODSFAILNTF_TYPE 23670

class PtcC2M_PayBuyGoodsFailNtf : public CProtocol
{
public:
    explicit PtcC2M_PayBuyGoodsFailNtf() : CProtocol(PTCC2M_PAYBUYGOODSFAILNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcC2M_PayBuyGoodsFailNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::PayBuyGoodsFail m_Data;
};

#endif