#ifndef __PTCG2C_MOBAROLECHANGENTF_H__
#define __PTCG2C_MOBAROLECHANGENTF_H__

// generate by ProtoGen at date: 2017/6/9 19:22:42

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_MOBAROLECHANGENTF_TYPE 12958

class PtcG2C_MobaRoleChangeNtf : public CProtocol
{
public:
    explicit PtcG2C_MobaRoleChangeNtf() : CProtocol(PTCG2C_MOBAROLECHANGENTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_MobaRoleChangeNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::MobaRoleChangeData m_Data;
};

#endif