#ifndef __PTCG2C_CLOSECHOOSEHERONTF_H__
#define __PTCG2C_CLOSECHOOSEHERONTF_H__

// generate by ProtoGen at date: 2016/12/27 20:22:52

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_CLOSECHOOSEHERONTF_TYPE 38670

class PtcG2C_CloseChooseHeroNtf : public CProtocol
{
public:
    explicit PtcG2C_CloseChooseHeroNtf() : CProtocol(PTCG2C_CLOSECHOOSEHERONTF_TYPE)
    {
		
    }

    virtual ~PtcG2C_CloseChooseHeroNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	
};

#endif