#ifndef __PTCG2C_BATTLESTATISTICSNTF_H__
#define __PTCG2C_BATTLESTATISTICSNTF_H__

// generate by ProtoGen at date: 2017/7/25 20:44:28

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_BATTLESTATISTICSNTF_TYPE 65061

class PtcG2C_BattleStatisticsNtf : public CProtocol
{
public:
    explicit PtcG2C_BattleStatisticsNtf() : CProtocol(PTCG2C_BATTLESTATISTICSNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_BattleStatisticsNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::BattleStatisticsNtf m_Data;
};

#endif