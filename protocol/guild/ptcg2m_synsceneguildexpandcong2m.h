#ifndef __PTCG2M_SYNSCENEGUILDEXPANDCONG2M_H__
#define __PTCG2M_SYNSCENEGUILDEXPANDCONG2M_H__

// generate by ProtoGen at date: 2016/8/5 19:24:46

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2M_SYNSCENEGUILDEXPANDCONG2M_TYPE 44844

class PtcG2M_SynSceneGuildExpAndConG2M : public CProtocol
{
public:
    explicit PtcG2M_SynSceneGuildExpAndConG2M() : CProtocol(PTCG2M_SYNSCENEGUILDEXPANDCONG2M_TYPE)
    {
		
    }

    virtual ~PtcG2M_SynSceneGuildExpAndConG2M()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	
};

#endif