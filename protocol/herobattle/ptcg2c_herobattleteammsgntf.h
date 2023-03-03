#ifndef __PTCG2C_HEROBATTLETEAMMSGNTF_H__
#define __PTCG2C_HEROBATTLETEAMMSGNTF_H__

// generate by ProtoGen at date: 2016/12/17 13:13:55

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_HEROBATTLETEAMMSGNTF_TYPE 1414

class PtcG2C_HeroBattleTeamMsgNtf : public CProtocol
{
public:
    explicit PtcG2C_HeroBattleTeamMsgNtf() : CProtocol(PTCG2C_HEROBATTLETEAMMSGNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_HeroBattleTeamMsgNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::HeroBattleTeamMsg m_Data;
};

#endif