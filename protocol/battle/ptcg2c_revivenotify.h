#ifndef __PTCG2C_REVIVENOTIFY_H__
#define __PTCG2C_REVIVENOTIFY_H__

// generate by ProtoGen at date: 2016/10/28 22:23:00

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_REVIVENOTIFY_TYPE 16213

class PtcG2C_ReviveNotify : public CProtocol
{
public:
    explicit PtcG2C_ReviveNotify() : CProtocol(PTCG2C_REVIVENOTIFY_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_ReviveNotify()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::ReviveInfo m_Data;
};

#endif