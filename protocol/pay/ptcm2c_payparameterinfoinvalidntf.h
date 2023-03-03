#ifndef __PTCM2C_PAYPARAMETERINFOINVALIDNTF_H__
#define __PTCM2C_PAYPARAMETERINFOINVALIDNTF_H__

// generate by ProtoGen at date: 2016/12/19 22:30:58

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2C_PAYPARAMETERINFOINVALIDNTF_TYPE 64504

class PtcM2C_PayParameterInfoInvalidNtf : public CProtocol
{
public:
    explicit PtcM2C_PayParameterInfoInvalidNtf() : CProtocol(PTCM2C_PAYPARAMETERINFOINVALIDNTF_TYPE)
    {
		
    }

    virtual ~PtcM2C_PayParameterInfoInvalidNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	
};

#endif