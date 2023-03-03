#ifndef __PTCG2C_SHARERANDOMGIFTNTF_H__
#define __PTCG2C_SHARERANDOMGIFTNTF_H__

// generate by ProtoGen at date: 2017/3/31 20:11:55

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_SHARERANDOMGIFTNTF_TYPE 18823

class PtcG2C_ShareRandomGiftNtf : public CProtocol
{
public:
    explicit PtcG2C_ShareRandomGiftNtf() : CProtocol(PTCG2C_SHARERANDOMGIFTNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_ShareRandomGiftNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::ShareRandomGiftData m_Data;
};

#endif