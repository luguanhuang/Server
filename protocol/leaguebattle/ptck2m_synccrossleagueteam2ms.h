#ifndef __PTCK2M_SYNCCROSSLEAGUETEAM2MS_H__
#define __PTCK2M_SYNCCROSSLEAGUETEAM2MS_H__

// generate by ProtoGen at date: 2017/2/2 15:09:13

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCK2M_SYNCCROSSLEAGUETEAM2MS_TYPE 12116

class PtcK2M_SyncCrossLeagueTeam2MS : public CProtocol
{
public:
    explicit PtcK2M_SyncCrossLeagueTeam2MS() : CProtocol(PTCK2M_SYNCCROSSLEAGUETEAM2MS_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcK2M_SyncCrossLeagueTeam2MS()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::SyncCrossLeagueTeam2MS m_Data;
};

#endif