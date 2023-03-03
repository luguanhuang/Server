#ifndef __PTCC2T_KEEPALIVEPINGACK_H__
#define __PTCC2T_KEEPALIVEPINGACK_H__

// generate by ProtoGen at date: 2016/12/15 17:32:50

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCC2T_KEEPALIVEPINGACK_TYPE 29192

class PtcC2T_KeepAlivePingAck : public CProtocol
{
public:
    explicit PtcC2T_KeepAlivePingAck() : CProtocol(PTCC2T_KEEPALIVEPINGACK_TYPE)
    {
		
    }

    virtual ~PtcC2T_KeepAlivePingAck()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	
};

#endif