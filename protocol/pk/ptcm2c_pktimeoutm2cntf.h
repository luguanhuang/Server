#ifndef __PTCM2C_PKTIMEOUTM2CNTF_H__
#define __PTCM2C_PKTIMEOUTM2CNTF_H__

// generate by ProtoGen at date: 2016/8/27 11:05:01

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2C_PKTIMEOUTM2CNTF_TYPE 4963

class PtcM2C_PkTimeoutM2CNtf : public CProtocol
{
public:
    explicit PtcM2C_PkTimeoutM2CNtf() : CProtocol(PTCM2C_PKTIMEOUTM2CNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2C_PkTimeoutM2CNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::PkTimeoutNtf m_Data;
};

#endif