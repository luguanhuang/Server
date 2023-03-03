#ifndef __PTCG2C_PKTIMEOUTNTF_H__
#define __PTCG2C_PKTIMEOUTNTF_H__

// generate by ProtoGen at date: 2015/12/11 14:37:59

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_PKTIMEOUTNTF_TYPE 58692

class PtcG2C_PkTimeoutNtf : public CProtocol
{
public:
    explicit PtcG2C_PkTimeoutNtf() : CProtocol(PTCG2C_PKTIMEOUTNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_PkTimeoutNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::PkTimeoutNtf m_Data;
};

#endif