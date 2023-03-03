#ifndef __PTCM2G_PAYTOTALNTF_H__
#define __PTCM2G_PAYTOTALNTF_H__

// generate by ProtoGen at date: 2017/10/19 20:30:24

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2G_PAYTOTALNTF_TYPE 11424

class PtcM2G_PayTotalNtf : public CProtocol
{
public:
    explicit PtcM2G_PayTotalNtf() : CProtocol(PTCM2G_PAYTOTALNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2G_PayTotalNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::PayTotalNtfData m_Data;
};

#endif