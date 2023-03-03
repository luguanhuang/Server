#ifndef __PTCM2G_NOTIFYDRAGONGUILDREDPOINT_H__
#define __PTCM2G_NOTIFYDRAGONGUILDREDPOINT_H__

// generate by ProtoGen at date: 2017/9/13 17:33:00

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2G_NOTIFYDRAGONGUILDREDPOINT_TYPE 28271

class PtcM2G_NotifyDragonGuildRedPoint : public CProtocol
{
public:
    explicit PtcM2G_NotifyDragonGuildRedPoint() : CProtocol(PTCM2G_NOTIFYDRAGONGUILDREDPOINT_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2G_NotifyDragonGuildRedPoint()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::NotifyDragonGuildRedPoint m_Data;
};

#endif