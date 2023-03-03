#ifndef __PTCG2C_PVPBATTLEKILL_H__
#define __PTCG2C_PVPBATTLEKILL_H__

// generate by ProtoGen at date: 2016/6/23 11:41:59

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_PVPBATTLEKILL_TYPE 61000

class PtcG2C_PvpBattleKill : public CProtocol
{
public:
    explicit PtcG2C_PvpBattleKill() : CProtocol(PTCG2C_PVPBATTLEKILL_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_PvpBattleKill()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::PvpBattleKill m_Data;
};

#endif