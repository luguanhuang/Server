#ifndef __PTCM2G_SYNCSERVERTIME_H__
#define __PTCM2G_SYNCSERVERTIME_H__

// generate by ProtoGen at date: 2016/11/10 17:42:19

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2G_SYNCSERVERTIME_TYPE 55598

class PtcM2G_syncservertime : public CProtocol
{
public:
    explicit PtcM2G_syncservertime() : CProtocol(PTCM2G_SYNCSERVERTIME_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2G_syncservertime()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::SyncServerTime m_Data;
};

#endif