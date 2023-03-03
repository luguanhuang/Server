#ifndef __PTCG2C_RESWARBATTLEDATANTF_H__
#define __PTCG2C_RESWARBATTLEDATANTF_H__

// generate by ProtoGen at date: 2016/11/1 14:44:04

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_RESWARBATTLEDATANTF_TYPE 18834

class PtcG2C_ResWarBattleDataNtf : public CProtocol
{
public:
    explicit PtcG2C_ResWarBattleDataNtf() : CProtocol(PTCG2C_RESWARBATTLEDATANTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_ResWarBattleDataNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::ResWarAllInfo m_Data;
};

#endif