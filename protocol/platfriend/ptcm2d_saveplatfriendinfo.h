#ifndef __PTCM2D_SAVEPLATFRIENDINFO_H__
#define __PTCM2D_SAVEPLATFRIENDINFO_H__

// generate by ProtoGen at date: 2016/10/31 19:43:39

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2D_SAVEPLATFRIENDINFO_TYPE 8055

class PtcM2D_SavePlatFriendInfo : public CProtocol
{
public:
    explicit PtcM2D_SavePlatFriendInfo() : CProtocol(PTCM2D_SAVEPLATFRIENDINFO_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2D_SavePlatFriendInfo()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::SavePlatFriendInfoData m_Data;
};

#endif