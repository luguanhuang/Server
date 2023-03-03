#ifndef __PTCG2C_HEROBATTLETEAMROLENTF_H__
#define __PTCG2C_HEROBATTLETEAMROLENTF_H__

// generate by ProtoGen at date: 2016/12/19 22:29:31

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_HEROBATTLETEAMROLENTF_TYPE 25720

class PtcG2C_HeroBattleTeamRoleNtf : public CProtocol
{
public:
    explicit PtcG2C_HeroBattleTeamRoleNtf() : CProtocol(PTCG2C_HEROBATTLETEAMROLENTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_HeroBattleTeamRoleNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::HeroBattleTeamRoleData m_Data;
};

#endif