#ifndef __PTCK2M_BROADCASTWORLD2MS_H__
#define __PTCK2M_BROADCASTWORLD2MS_H__

// generate by ProtoGen at date: 2017/2/18 21:16:54

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCK2M_BROADCASTWORLD2MS_TYPE 34278

class PtcK2M_BroadCastWorld2Ms : public CProtocol
{
public:
    explicit PtcK2M_BroadCastWorld2Ms() : CProtocol(PTCK2M_BROADCASTWORLD2MS_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcK2M_BroadCastWorld2Ms()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::WorldMasterInfo m_Data;
};

#endif