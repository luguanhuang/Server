#ifndef __PTCG2C_BATTLEFIELDRELIVENTF_H__
#define __PTCG2C_BATTLEFIELDRELIVENTF_H__

// generate by ProtoGen at date: 2017/9/4 16:20:19

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_BATTLEFIELDRELIVENTF_TYPE 813

class PtcG2C_BattleFieldReliveNtf : public CProtocol
{
public:
    explicit PtcG2C_BattleFieldReliveNtf() : CProtocol(PTCG2C_BATTLEFIELDRELIVENTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_BattleFieldReliveNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::BattleFieldReliveInfo m_Data;
};

#endif