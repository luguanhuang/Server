#ifndef __PTCK2M_SYNCSERVERTIME2MS_H__
#define __PTCK2M_SYNCSERVERTIME2MS_H__

// generate by ProtoGen at date: 2017/2/5 16:53:22

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCK2M_SYNCSERVERTIME2MS_TYPE 37391

class PtcK2M_SyncServerTime2MS : public CProtocol
{
public:
    explicit PtcK2M_SyncServerTime2MS() : CProtocol(PTCK2M_SYNCSERVERTIME2MS_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcK2M_SyncServerTime2MS()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::SyncServerTime m_Data;
};

#endif