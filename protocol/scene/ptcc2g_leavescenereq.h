#ifndef __PTCC2G_LEAVESCENEREQ_H__
#define __PTCC2G_LEAVESCENEREQ_H__

// generate by ProtoGen at date: 2014/10/16 11:11:07

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCC2G_LEAVESCENEREQ_TYPE 27927

class PtcC2G_LeaveSceneReq : public CProtocol
{
public:
    explicit PtcC2G_LeaveSceneReq() : CProtocol(PTCC2G_LEAVESCENEREQ_TYPE)
    {
		
    }

    virtual ~PtcC2G_LeaveSceneReq()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	
};

#endif