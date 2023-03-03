#ifndef __PTCG2C_BATTLERESULTNTF_H__
#define __PTCG2C_BATTLERESULTNTF_H__

// generate by ProtoGen at date: 2016/4/20 11:58:35

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_BATTLERESULTNTF_TYPE 29609

class PtcG2C_BattleResultNtf : public CProtocol
{
public:
    explicit PtcG2C_BattleResultNtf() : CProtocol(PTCG2C_BATTLERESULTNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_BattleResultNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::NewBattleResult m_Data;
};

#endif