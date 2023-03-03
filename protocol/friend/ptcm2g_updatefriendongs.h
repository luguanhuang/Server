#ifndef __PTCM2G_UPDATEFRIENDONGS_H__
#define __PTCM2G_UPDATEFRIENDONGS_H__

// generate by ProtoGen at date: 2016/7/30 20:53:11

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2G_UPDATEFRIENDONGS_TYPE 50058

class PtcM2G_UpdateFriendOnGS : public CProtocol
{
public:
    explicit PtcM2G_UpdateFriendOnGS() : CProtocol(PTCM2G_UPDATEFRIENDONGS_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2G_UpdateFriendOnGS()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::UpdateFriendOnGS m_Data;
};

#endif