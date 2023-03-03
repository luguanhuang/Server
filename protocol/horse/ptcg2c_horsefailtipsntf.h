#ifndef __PTCG2C_HORSEFAILTIPSNTF_H__
#define __PTCG2C_HORSEFAILTIPSNTF_H__

// generate by ProtoGen at date: 2017/7/26 17:24:18

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_HORSEFAILTIPSNTF_TYPE 2357

class PtcG2C_HorseFailTipsNtf : public CProtocol
{
public:
    explicit PtcG2C_HorseFailTipsNtf() : CProtocol(PTCG2C_HORSEFAILTIPSNTF_TYPE)
    {
		
    }

    virtual ~PtcG2C_HorseFailTipsNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	
};

#endif