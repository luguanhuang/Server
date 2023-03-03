#ifndef __PTCG2C_PVPBATTLEBEGINNTF_H__
#define __PTCG2C_PVPBATTLEBEGINNTF_H__

// generate by ProtoGen at date: 2016/6/23 11:41:05

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_PVPBATTLEBEGINNTF_TYPE 53763

class PtcG2C_PvpBattleBeginNtf : public CProtocol
{
public:
    explicit PtcG2C_PvpBattleBeginNtf() : CProtocol(PTCG2C_PVPBATTLEBEGINNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_PvpBattleBeginNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::PvpBattleBeginData m_Data;
};

#endif