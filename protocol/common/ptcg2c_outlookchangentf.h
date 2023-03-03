#ifndef __PTCG2C_OUTLOOKCHANGENTF_H__
#define __PTCG2C_OUTLOOKCHANGENTF_H__

// generate by ProtoGen at date: 2016/6/16 14:25:16

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_OUTLOOKCHANGENTF_TYPE 28395

class PtcG2C_OutLookChangeNtf : public CProtocol
{
public:
    explicit PtcG2C_OutLookChangeNtf() : CProtocol(PTCG2C_OUTLOOKCHANGENTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_OutLookChangeNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::OutLookChange m_Data;
};

#endif