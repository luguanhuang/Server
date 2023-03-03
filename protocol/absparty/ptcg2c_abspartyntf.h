#ifndef __PTCG2C_ABSPARTYNTF_H__
#define __PTCG2C_ABSPARTYNTF_H__

// generate by ProtoGen at date: 2017/5/5 20:46:54

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_ABSPARTYNTF_TYPE 35041

class PtcG2C_AbsPartyNtf : public CProtocol
{
public:
    explicit PtcG2C_AbsPartyNtf() : CProtocol(PTCG2C_ABSPARTYNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_AbsPartyNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::AbsPartyInfo m_Data;
};

#endif