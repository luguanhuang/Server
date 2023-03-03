#ifndef __PTCG2C_PVPBATTLEENDNTF_H__
#define __PTCG2C_PVPBATTLEENDNTF_H__

// generate by ProtoGen at date: 2016/6/23 11:41:30

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_PVPBATTLEENDNTF_TYPE 46438

class PtcG2C_PvpBattleEndNtf : public CProtocol
{
public:
    explicit PtcG2C_PvpBattleEndNtf() : CProtocol(PTCG2C_PVPBATTLEENDNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_PvpBattleEndNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::PvpBattleEndData m_Data;
};

#endif