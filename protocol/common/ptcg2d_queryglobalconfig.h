#ifndef __PTCG2D_QUERYGLOBALCONFIG_H__
#define __PTCG2D_QUERYGLOBALCONFIG_H__

// generate by ProtoGen at date: 2015/6/9 14:19:05

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2D_QUERYGLOBALCONFIG_TYPE 58270

class PtcG2D_QueryGlobalConfig : public CProtocol
{
public:
    explicit PtcG2D_QueryGlobalConfig() : CProtocol(PTCG2D_QUERYGLOBALCONFIG_TYPE)
    {
		
    }

    virtual ~PtcG2D_QueryGlobalConfig()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	
};

#endif