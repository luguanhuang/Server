#ifndef __PTCM2C_NOTIFYLEAGUETEAMCREATE_H__
#define __PTCM2C_NOTIFYLEAGUETEAMCREATE_H__

// generate by ProtoGen at date: 2017/1/5 22:58:11

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2C_NOTIFYLEAGUETEAMCREATE_TYPE 22343

class PtcM2C_NotifyLeagueTeamCreate : public CProtocol
{
public:
    explicit PtcM2C_NotifyLeagueTeamCreate() : CProtocol(PTCM2C_NOTIFYLEAGUETEAMCREATE_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2C_NotifyLeagueTeamCreate()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::NotifyLeagueTeamCreate m_Data;
};

#endif