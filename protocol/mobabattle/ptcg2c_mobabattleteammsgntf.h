#ifndef __PTCG2C_MOBABATTLETEAMMSGNTF_H__
#define __PTCG2C_MOBABATTLETEAMMSGNTF_H__

// generate by ProtoGen at date: 2017/6/10 17:13:05

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_MOBABATTLETEAMMSGNTF_TYPE 14987

class PtcG2C_MobaBattleTeamMsgNtf : public CProtocol
{
public:
    explicit PtcG2C_MobaBattleTeamMsgNtf() : CProtocol(PTCG2C_MOBABATTLETEAMMSGNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_MobaBattleTeamMsgNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::MobaBattleTeamMsg m_Data;
};

#endif