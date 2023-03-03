#ifndef __PTCG2C_FLOWERRANKREWARDNTF_H__
#define __PTCG2C_FLOWERRANKREWARDNTF_H__

// generate by ProtoGen at date: 2016/6/25 17:03:17

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_FLOWERRANKREWARDNTF_TYPE 14326

class PtcG2C_FlowerRankRewardNtf : public CProtocol
{
public:
    explicit PtcG2C_FlowerRankRewardNtf() : CProtocol(PTCG2C_FLOWERRANKREWARDNTF_TYPE)
    {
		
    }

    virtual ~PtcG2C_FlowerRankRewardNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	
};

#endif