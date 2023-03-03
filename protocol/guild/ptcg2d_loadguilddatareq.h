#ifndef __PTCG2D_LOADGUILDDATAREQ_H__
#define __PTCG2D_LOADGUILDDATAREQ_H__

// generate by ProtoGen at date: 2015/10/2 16:28:24

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2D_LOADGUILDDATAREQ_TYPE 39792

class PtcG2D_LoadGuildDataReq : public CProtocol
{
public:
    explicit PtcG2D_LoadGuildDataReq() : CProtocol(PTCG2D_LOADGUILDDATAREQ_TYPE)
    {
		
    }

    virtual ~PtcG2D_LoadGuildDataReq()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	
};

#endif