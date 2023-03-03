#ifndef __PTCC2G_ROLEDEATHINSOLOSCENE_H__
#define __PTCC2G_ROLEDEATHINSOLOSCENE_H__

// generate by ProtoGen at date: 2016/10/28 22:23:02

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCC2G_ROLEDEATHINSOLOSCENE_TYPE 16659

class PtcC2G_RoleDeathInSoloScene : public CProtocol
{
public:
    explicit PtcC2G_RoleDeathInSoloScene() : CProtocol(PTCC2G_ROLEDEATHINSOLOSCENE_TYPE)
    {
		
    }

    virtual ~PtcC2G_RoleDeathInSoloScene()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	
};

#endif