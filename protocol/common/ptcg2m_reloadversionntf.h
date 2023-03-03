#ifndef __PTCG2M_RELOADVERSIONNTF_H__
#define __PTCG2M_RELOADVERSIONNTF_H__

// generate by ProtoGen at date: 2016/8/10 0:16:39

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2M_RELOADVERSIONNTF_TYPE 44559

class PtcG2M_ReloadVersionNtf : public CProtocol
{
public:
    explicit PtcG2M_ReloadVersionNtf() : CProtocol(PTCG2M_RELOADVERSIONNTF_TYPE)
    {
		
    }

    virtual ~PtcG2M_ReloadVersionNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	
};

#endif