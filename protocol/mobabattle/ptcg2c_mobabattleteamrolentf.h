#ifndef __PTCG2C_MOBABATTLETEAMROLENTF_H__
#define __PTCG2C_MOBABATTLETEAMROLENTF_H__

// generate by ProtoGen at date: 2017/6/9 16:25:34

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_MOBABATTLETEAMROLENTF_TYPE 44930

class PtcG2C_MobaBattleTeamRoleNtf : public CProtocol
{
public:
    explicit PtcG2C_MobaBattleTeamRoleNtf() : CProtocol(PTCG2C_MOBABATTLETEAMROLENTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_MobaBattleTeamRoleNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::MobaBattleTeamRoleData m_Data;
};

#endif