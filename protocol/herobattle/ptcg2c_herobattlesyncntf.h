#ifndef __PTCG2C_HEROBATTLESYNCNTF_H__
#define __PTCG2C_HEROBATTLESYNCNTF_H__

// generate by ProtoGen at date: 2016/12/25 18:50:47

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_HEROBATTLESYNCNTF_TYPE 33024

class PtcG2C_HeroBattleSyncNtf : public CProtocol
{
public:
    explicit PtcG2C_HeroBattleSyncNtf() : CProtocol(PTCG2C_HEROBATTLESYNCNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_HeroBattleSyncNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::HeroBattleSyncData m_Data;
};

#endif