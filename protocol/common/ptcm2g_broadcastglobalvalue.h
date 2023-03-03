#ifndef __PTCM2G_BROADCASTGLOBALVALUE_H__
#define __PTCM2G_BROADCASTGLOBALVALUE_H__

// generate by ProtoGen at date: 2017/4/23 22:30:02

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2G_BROADCASTGLOBALVALUE_TYPE 22233

class PtcM2G_BroadCastGlobalValue : public CProtocol
{
public:
    explicit PtcM2G_BroadCastGlobalValue() : CProtocol(PTCM2G_BROADCASTGLOBALVALUE_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2G_BroadCastGlobalValue()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::BroadCastGlobalValue m_Data;
};

#endif