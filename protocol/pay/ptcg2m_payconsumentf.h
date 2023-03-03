#ifndef __PTCG2M_PAYCONSUMENTF_H__
#define __PTCG2M_PAYCONSUMENTF_H__

// generate by ProtoGen at date: 2016/12/26 20:21:49

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2M_PAYCONSUMENTF_TYPE 1991

class PtcG2M_PayConsumeNtf : public CProtocol
{
public:
    explicit PtcG2M_PayConsumeNtf() : CProtocol(PTCG2M_PAYCONSUMENTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2M_PayConsumeNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::PayConsumeDiamondInfo m_Data;
};

#endif