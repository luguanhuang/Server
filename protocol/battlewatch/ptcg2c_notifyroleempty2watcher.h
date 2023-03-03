#ifndef __PTCG2C_NOTIFYROLEEMPTY2WATCHER_H__
#define __PTCG2C_NOTIFYROLEEMPTY2WATCHER_H__

// generate by ProtoGen at date: 2016/7/21 15:51:34

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_NOTIFYROLEEMPTY2WATCHER_TYPE 1540

class PtcG2C_NotifyRoleEmpty2Watcher : public CProtocol
{
public:
    explicit PtcG2C_NotifyRoleEmpty2Watcher() : CProtocol(PTCG2C_NOTIFYROLEEMPTY2WATCHER_TYPE)
    {
		
    }

    virtual ~PtcG2C_NotifyRoleEmpty2Watcher()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	
};

#endif