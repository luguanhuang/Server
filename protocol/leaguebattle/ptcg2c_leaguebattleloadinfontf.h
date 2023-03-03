#ifndef __PTCG2C_LEAGUEBATTLELOADINFONTF_H__
#define __PTCG2C_LEAGUEBATTLELOADINFONTF_H__

// generate by ProtoGen at date: 2017/1/9 7:18:13

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_LEAGUEBATTLELOADINFONTF_TYPE 16091

class PtcG2C_LeagueBattleLoadInfoNtf : public CProtocol
{
public:
    explicit PtcG2C_LeagueBattleLoadInfoNtf() : CProtocol(PTCG2C_LEAGUEBATTLELOADINFONTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_LeagueBattleLoadInfoNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::LeagueBattleLoadInfoNtf m_Data;
};

#endif