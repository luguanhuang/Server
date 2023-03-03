#ifndef __PTCK2R_BROADCASTWORLD2RS_H__
#define __PTCK2R_BROADCASTWORLD2RS_H__

// generate by ProtoGen at date: 2017/2/18 21:17:24

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCK2R_BROADCASTWORLD2RS_TYPE 29151

class PtcK2R_BroadCastWorld2Rs : public CProtocol
{
public:
    explicit PtcK2R_BroadCastWorld2Rs() : CProtocol(PTCK2R_BROADCASTWORLD2RS_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcK2R_BroadCastWorld2Rs()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::WorldMasterInfo m_Data;
};

#endif