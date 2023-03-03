#ifndef __PTCC2G_SYNCSCENEFINISH_H__
#define __PTCC2G_SYNCSCENEFINISH_H__

// generate by ProtoGen at date: 2016/6/16 14:25:25

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCC2G_SYNCSCENEFINISH_TYPE 559

class PtcC2G_SyncSceneFinish : public CProtocol
{
public:
    explicit PtcC2G_SyncSceneFinish() : CProtocol(PTCC2G_SYNCSCENEFINISH_TYPE)
    {
		
    }

    virtual ~PtcC2G_SyncSceneFinish()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	
};

#endif