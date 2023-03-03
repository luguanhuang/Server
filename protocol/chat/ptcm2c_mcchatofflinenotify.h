#ifndef __PTCM2C_MCCHATOFFLINENOTIFY_H__
#define __PTCM2C_MCCHATOFFLINENOTIFY_H__

// generate by ProtoGen at date: 2016/8/2 8:13:02

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2C_MCCHATOFFLINENOTIFY_TYPE 35008

class PtcM2C_MCChatOffLineNotify : public CProtocol
{
public:
    explicit PtcM2C_MCChatOffLineNotify() : CProtocol(PTCM2C_MCCHATOFFLINENOTIFY_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2C_MCChatOffLineNotify()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::ChatOfflineNotify m_Data;
};

#endif