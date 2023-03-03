#ifndef __PTCG2C_LEAGUEBATTLEBASEDATANTF_H__
#define __PTCG2C_LEAGUEBATTLEBASEDATANTF_H__

// generate by ProtoGen at date: 2017/1/9 7:51:58

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_LEAGUEBATTLEBASEDATANTF_TYPE 19581

class PtcG2C_LeagueBattleBaseDataNtf : public CProtocol
{
public:
    explicit PtcG2C_LeagueBattleBaseDataNtf() : CProtocol(PTCG2C_LEAGUEBATTLEBASEDATANTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_LeagueBattleBaseDataNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::LeagueBattleBaseDataNtf m_Data;
};

#endif