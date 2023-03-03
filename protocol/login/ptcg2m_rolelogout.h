#ifndef __PTCG2M_ROLELOGOUT_H__
#define __PTCG2M_ROLELOGOUT_H__

// generate by ProtoGen at date: 2016/11/26 16:38:24

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2M_ROLELOGOUT_TYPE 64285

class PtcG2M_RoleLogout : public CProtocol
{
public:
    explicit PtcG2M_RoleLogout() : CProtocol(PTCG2M_ROLELOGOUT_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2M_RoleLogout()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::RoleLogoutData m_Data;
};

#endif