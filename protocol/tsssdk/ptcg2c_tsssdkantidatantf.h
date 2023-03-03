#ifndef __PTCG2C_TSSSDKANTIDATANTF_H__
#define __PTCG2C_TSSSDKANTIDATANTF_H__

// generate by ProtoGen at date: 2016/6/22 15:52:11

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_TSSSDKANTIDATANTF_TYPE 33482

class PtcG2C_TssSdkAntiDataNtf : public CProtocol
{
public:
    explicit PtcG2C_TssSdkAntiDataNtf() : CProtocol(PTCG2C_TSSSDKANTIDATANTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_TssSdkAntiDataNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::TssSdkAntiData m_Data;
};

#endif