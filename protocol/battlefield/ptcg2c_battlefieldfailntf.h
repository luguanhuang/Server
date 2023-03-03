#ifndef __PTCG2C_BATTLEFIELDFAILNTF_H__
#define __PTCG2C_BATTLEFIELDFAILNTF_H__

// generate by ProtoGen at date: 2017/9/4 16:12:51

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_BATTLEFIELDFAILNTF_TYPE 56799

class PtcG2C_BattleFieldFailNtf : public CProtocol
{
public:
    explicit PtcG2C_BattleFieldFailNtf() : CProtocol(PTCG2C_BATTLEFIELDFAILNTF_TYPE)
    {
		
    }

    virtual ~PtcG2C_BattleFieldFailNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	
};

#endif