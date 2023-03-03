﻿#ifndef __PTCG2C_PLATFORMSHAREAWARDNTF_H__
#define __PTCG2C_PLATFORMSHAREAWARDNTF_H__

// generate by ProtoGen at date: 2017/5/31 20:52:23

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_PLATFORMSHAREAWARDNTF_TYPE 24055

class PtcG2C_PlatformShareAwardNtf : public CProtocol
{
public:
    explicit PtcG2C_PlatformShareAwardNtf() : CProtocol(PTCG2C_PLATFORMSHAREAWARDNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_PlatformShareAwardNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::PlatformShareAwardPara m_Data;
};

#endif