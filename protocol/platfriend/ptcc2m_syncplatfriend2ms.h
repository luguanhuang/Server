#ifndef __PTCC2M_SYNCPLATFRIEND2MS_H__
#define __PTCC2M_SYNCPLATFRIEND2MS_H__

// generate by ProtoGen at date: 2016/10/27 10:54:00

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCC2M_SYNCPLATFRIEND2MS_TYPE 38885

class PtcC2M_SyncPlatFriend2MS : public CProtocol
{
public:
    explicit PtcC2M_SyncPlatFriend2MS() : CProtocol(PTCC2M_SYNCPLATFRIEND2MS_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcC2M_SyncPlatFriend2MS()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::SyncPlatFriend2MSData m_Data;
};

#endif