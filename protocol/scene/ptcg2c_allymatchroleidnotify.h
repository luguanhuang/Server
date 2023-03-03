#ifndef __PTCG2C_ALLYMATCHROLEIDNOTIFY_H__
#define __PTCG2C_ALLYMATCHROLEIDNOTIFY_H__

// generate by ProtoGen at date: 2016/3/24 16:49:56

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_ALLYMATCHROLEIDNOTIFY_TYPE 41598

class PtcG2C_AllyMatchRoleIDNotify : public CProtocol
{
public:
    explicit PtcG2C_AllyMatchRoleIDNotify() : CProtocol(PTCG2C_ALLYMATCHROLEIDNOTIFY_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_AllyMatchRoleIDNotify()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::AllyMatchRoleID m_Data;
};

#endif