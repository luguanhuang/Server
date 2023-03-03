#ifndef __PTCG2C_LEAVESCENENTF_H__
#define __PTCG2C_LEAVESCENENTF_H__

// generate by ProtoGen at date: 2014/10/16 16:05:40

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_LEAVESCENENTF_TYPE 33831

class PtcG2C_LeaveSceneNtf : public CProtocol
{
public:
    explicit PtcG2C_LeaveSceneNtf() : CProtocol(PTCG2C_LEAVESCENENTF_TYPE)
    {
		
    }

    virtual ~PtcG2C_LeaveSceneNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	
};

#endif