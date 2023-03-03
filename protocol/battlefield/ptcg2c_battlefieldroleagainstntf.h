#ifndef __PTCG2C_BATTLEFIELDROLEAGAINSTNTF_H__
#define __PTCG2C_BATTLEFIELDROLEAGAINSTNTF_H__

// generate by ProtoGen at date: 2017/9/4 15:56:24

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_BATTLEFIELDROLEAGAINSTNTF_TYPE 8049

class PtcG2C_BattleFieldRoleAgainstNtf : public CProtocol
{
public:
    explicit PtcG2C_BattleFieldRoleAgainstNtf() : CProtocol(PTCG2C_BATTLEFIELDROLEAGAINSTNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_BattleFieldRoleAgainstNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::BattleFieldRoleAgainst m_Data;
};

#endif