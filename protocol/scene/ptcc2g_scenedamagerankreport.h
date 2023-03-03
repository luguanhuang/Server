#ifndef __PTCC2G_SCENEDAMAGERANKREPORT_H__
#define __PTCC2G_SCENEDAMAGERANKREPORT_H__

// generate by ProtoGen at date: 2015/12/24 17:43:22

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCC2G_SCENEDAMAGERANKREPORT_TYPE 53015

class PtcC2G_SceneDamageRankReport : public CProtocol
{
public:
    explicit PtcC2G_SceneDamageRankReport() : CProtocol(PTCC2G_SCENEDAMAGERANKREPORT_TYPE)
    {
		
    }

    virtual ~PtcC2G_SceneDamageRankReport()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	
};

#endif