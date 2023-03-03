#ifndef __PTCM2C_KICKFMAUCHOR_H__
#define __PTCM2C_KICKFMAUCHOR_H__

// generate by ProtoGen at date: 2017/1/16 14:29:26

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2C_KICKFMAUCHOR_TYPE 33806

class PtcM2C_KickFMAuchor : public CProtocol
{
public:
    explicit PtcM2C_KickFMAuchor() : CProtocol(PTCM2C_KICKFMAUCHOR_TYPE)
    {
		
    }

    virtual ~PtcM2C_KickFMAuchor()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	
};

#endif