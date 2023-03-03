#ifndef __PTCG2M_ROLELOGINRECONNECTNTF_H__
#define __PTCG2M_ROLELOGINRECONNECTNTF_H__

// generate by ProtoGen at date: 2017/6/21 17:26:57

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2M_ROLELOGINRECONNECTNTF_TYPE 47084

class PtcG2M_RoleLoginReconnectNtf : public CProtocol
{
public:
    explicit PtcG2M_RoleLoginReconnectNtf() : CProtocol(PTCG2M_ROLELOGINRECONNECTNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2M_RoleLoginReconnectNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::RoleLoginReconnectNtf m_Data;
};

#endif