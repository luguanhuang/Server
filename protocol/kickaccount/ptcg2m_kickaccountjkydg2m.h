#ifndef __PTCG2M_KICKACCOUNTJKYDG2M_H__
#define __PTCG2M_KICKACCOUNTJKYDG2M_H__

// generate by ProtoGen at date: 2017/10/12 15:53:21

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2M_KICKACCOUNTJKYDG2M_TYPE 65490

class PtcG2M_KickAccountJkydG2M : public CProtocol
{
public:
    explicit PtcG2M_KickAccountJkydG2M() : CProtocol(PTCG2M_KICKACCOUNTJKYDG2M_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2M_KickAccountJkydG2M()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::KickAccountJkydG2M m_Data;
};

#endif