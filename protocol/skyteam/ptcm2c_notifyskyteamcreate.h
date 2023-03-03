#ifndef __PTCM2C_NOTIFYSKYTEAMCREATE_H__
#define __PTCM2C_NOTIFYSKYTEAMCREATE_H__

// generate by ProtoGen at date: 2017/3/31 22:15:17

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2C_NOTIFYSKYTEAMCREATE_TYPE 21688

class PtcM2C_NotifySkyTeamCreate : public CProtocol
{
public:
    explicit PtcM2C_NotifySkyTeamCreate() : CProtocol(PTCM2C_NOTIFYSKYTEAMCREATE_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2C_NotifySkyTeamCreate()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::NotifySkyTeamCreate m_Data;
};

#endif