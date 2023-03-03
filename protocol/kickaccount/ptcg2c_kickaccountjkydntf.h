#ifndef __PTCG2C_KICKACCOUNTJKYDNTF_H__
#define __PTCG2C_KICKACCOUNTJKYDNTF_H__

// generate by ProtoGen at date: 2017/10/12 15:51:53

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_KICKACCOUNTJKYDNTF_TYPE 39286

class PtcG2C_KickAccountJkydNtf : public CProtocol
{
public:
    explicit PtcG2C_KickAccountJkydNtf() : CProtocol(PTCG2C_KICKACCOUNTJKYDNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_KickAccountJkydNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::KickAccountJkydMsg m_Data;
};

#endif