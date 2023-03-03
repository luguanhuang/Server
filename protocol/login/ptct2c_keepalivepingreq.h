#ifndef __PTCT2C_KEEPALIVEPINGREQ_H__
#define __PTCT2C_KEEPALIVEPINGREQ_H__

// generate by ProtoGen at date: 2016/12/16 16:00:45

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCT2C_KEEPALIVEPINGREQ_TYPE 49142

class PtcT2C_KeepAlivePingReq : public CProtocol
{
public:
    explicit PtcT2C_KeepAlivePingReq() : CProtocol(PTCT2C_KEEPALIVEPINGREQ_TYPE)
    {
		
    }

    virtual ~PtcT2C_KeepAlivePingReq()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	
};

#endif