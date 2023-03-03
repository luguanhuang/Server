#ifndef __PTCG2C_BATTLEWATCHERNTF_H__
#define __PTCG2C_BATTLEWATCHERNTF_H__

// generate by ProtoGen at date: 2016/7/22 10:13:36

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_BATTLEWATCHERNTF_TYPE 54652

class PtcG2C_BattleWatcherNtf : public CProtocol
{
public:
    explicit PtcG2C_BattleWatcherNtf() : CProtocol(PTCG2C_BATTLEWATCHERNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_BattleWatcherNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::BattleWatcherNtf m_Data;
};

#endif