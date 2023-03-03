#ifndef __PTCG2M_PAYADDDIAMONDNTF_H__
#define __PTCG2M_PAYADDDIAMONDNTF_H__

// generate by ProtoGen at date: 2016/12/22 16:27:07

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2M_PAYADDDIAMONDNTF_TYPE 40400

class PtcG2M_PayAddDiamondNtf : public CProtocol
{
public:
    explicit PtcG2M_PayAddDiamondNtf() : CProtocol(PTCG2M_PAYADDDIAMONDNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2M_PayAddDiamondNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::PayAddDiamondInfo m_Data;
};

#endif