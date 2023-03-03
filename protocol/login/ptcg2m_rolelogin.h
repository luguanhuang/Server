#ifndef __PTCG2M_ROLELOGIN_H__
#define __PTCG2M_ROLELOGIN_H__

// generate by ProtoGen at date: 2016/11/26 16:38:19

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2M_ROLELOGIN_TYPE 39016

class PtcG2M_RoleLogin : public CProtocol
{
public:
    explicit PtcG2M_RoleLogin() : CProtocol(PTCG2M_ROLELOGIN_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2M_RoleLogin()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::RoleLoginData m_Data;
};

#endif