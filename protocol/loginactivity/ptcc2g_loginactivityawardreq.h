#ifndef __PTCC2G_LOGINACTIVITYAWARDREQ_H__
#define __PTCC2G_LOGINACTIVITYAWARDREQ_H__

// generate by ProtoGen at date: 2017/4/26 19:27:55

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCC2G_LOGINACTIVITYAWARDREQ_TYPE 16542

class PtcC2G_LoginActivityAwardReq : public CProtocol
{
public:
    explicit PtcC2G_LoginActivityAwardReq() : CProtocol(PTCC2G_LOGINACTIVITYAWARDREQ_TYPE)
    {
		
    }

    virtual ~PtcC2G_LoginActivityAwardReq()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	
};

#endif