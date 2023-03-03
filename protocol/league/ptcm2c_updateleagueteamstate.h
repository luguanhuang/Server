#ifndef __PTCM2C_UPDATELEAGUETEAMSTATE_H__
#define __PTCM2C_UPDATELEAGUETEAMSTATE_H__

// generate by ProtoGen at date: 2017/1/14 22:15:04

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2C_UPDATELEAGUETEAMSTATE_TYPE 7643

class PtcM2C_UpdateLeagueTeamState : public CProtocol
{
public:
    explicit PtcM2C_UpdateLeagueTeamState() : CProtocol(PTCM2C_UPDATELEAGUETEAMSTATE_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2C_UpdateLeagueTeamState()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::UpdateLeagueTeamState m_Data;
};

#endif