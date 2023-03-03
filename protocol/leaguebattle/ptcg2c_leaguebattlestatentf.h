#ifndef __PTCG2C_LEAGUEBATTLESTATENTF_H__
#define __PTCG2C_LEAGUEBATTLESTATENTF_H__

// generate by ProtoGen at date: 2017/1/10 18:13:41

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_LEAGUEBATTLESTATENTF_TYPE 59496

class PtcG2C_LeagueBattleStateNtf : public CProtocol
{
public:
    explicit PtcG2C_LeagueBattleStateNtf() : CProtocol(PTCG2C_LEAGUEBATTLESTATENTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_LeagueBattleStateNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::LeagueBattleStateNtf m_Data;
};

#endif