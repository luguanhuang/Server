#ifndef __PTCG2C_ACTIVITYROLENOTIFY_H__
#define __PTCG2C_ACTIVITYROLENOTIFY_H__

// generate by ProtoGen at date: 2015/6/27 12:23:32

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_ACTIVITYROLENOTIFY_TYPE 2548

class PtcG2C_ActivityRoleNotify : public CProtocol
{
public:
    explicit PtcG2C_ActivityRoleNotify() : CProtocol(PTCG2C_ACTIVITYROLENOTIFY_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_ActivityRoleNotify()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::ActivityRoleNotify m_Data;
};

#endif