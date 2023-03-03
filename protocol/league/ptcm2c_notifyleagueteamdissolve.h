#ifndef __PTCM2C_NOTIFYLEAGUETEAMDISSOLVE_H__
#define __PTCM2C_NOTIFYLEAGUETEAMDISSOLVE_H__

// generate by ProtoGen at date: 2017/1/5 9:45:49

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2C_NOTIFYLEAGUETEAMDISSOLVE_TYPE 11033

class PtcM2C_NotifyLeagueTeamDissolve : public CProtocol
{
public:
    explicit PtcM2C_NotifyLeagueTeamDissolve() : CProtocol(PTCM2C_NOTIFYLEAGUETEAMDISSOLVE_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2C_NotifyLeagueTeamDissolve()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::NotifyLeagueTeamDissolve m_Data;
};

#endif