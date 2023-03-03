#ifndef __PTCG2C_SKYCITYTEAMRES_H__
#define __PTCG2C_SKYCITYTEAMRES_H__

// generate by ProtoGen at date: 2016/9/30 21:05:23

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_SKYCITYTEAMRES_TYPE 49519

class PtcG2C_SkyCityTeamRes : public CProtocol
{
public:
    explicit PtcG2C_SkyCityTeamRes() : CProtocol(PTCG2C_SKYCITYTEAMRES_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_SkyCityTeamRes()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::SkyCityAllTeamBaseInfo m_Data;
};

#endif