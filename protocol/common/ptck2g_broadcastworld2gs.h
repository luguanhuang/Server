#ifndef __PTCK2G_BROADCASTWORLD2GS_H__
#define __PTCK2G_BROADCASTWORLD2GS_H__

// generate by ProtoGen at date: 2017/2/18 21:16:35

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCK2G_BROADCASTWORLD2GS_TYPE 20476

class PtcK2G_BroadCastWorld2Gs : public CProtocol
{
public:
    explicit PtcK2G_BroadCastWorld2Gs() : CProtocol(PTCK2G_BROADCASTWORLD2GS_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcK2G_BroadCastWorld2Gs()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::WorldMasterInfo m_Data;
};

#endif