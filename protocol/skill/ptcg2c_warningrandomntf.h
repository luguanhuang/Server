#ifndef __PTCG2C_WARNINGRANDOMNTF_H__
#define __PTCG2C_WARNINGRANDOMNTF_H__

// generate by ProtoGen at date: 2017/7/27 14:55:37

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_WARNINGRANDOMNTF_TYPE 8594

class PtcG2C_WarningRandomNtf : public CProtocol
{
public:
    explicit PtcG2C_WarningRandomNtf() : CProtocol(PTCG2C_WARNINGRANDOMNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_WarningRandomNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::WarningRandomSet m_Data;
};

#endif