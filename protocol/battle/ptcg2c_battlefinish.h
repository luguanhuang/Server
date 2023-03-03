#ifndef __PTCG2C_BATTLEFINISH_H__
#define __PTCG2C_BATTLEFINISH_H__

// generate by ProtoGen at date: 2015/7/18 14:44:24

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_BATTLEFINISH_TYPE 23809

class PtcG2C_BattleFinish : public CProtocol
{
public:
    explicit PtcG2C_BattleFinish() : CProtocol(PTCG2C_BATTLEFINISH_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_BattleFinish()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::BattleFinishResult m_Data;
};

#endif