#ifndef __PTCG2C_LEAGUEBATTLERESULTNTF_H__
#define __PTCG2C_LEAGUEBATTLERESULTNTF_H__

// generate by ProtoGen at date: 2017/1/9 8:27:20

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_LEAGUEBATTLERESULTNTF_TYPE 29255

class PtcG2C_LeagueBattleResultNtf : public CProtocol
{
public:
    explicit PtcG2C_LeagueBattleResultNtf() : CProtocol(PTCG2C_LEAGUEBATTLERESULTNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_LeagueBattleResultNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::LeagueBattleResultNtf m_Data;
};

#endif