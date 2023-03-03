#ifndef __PTCM2C_LEAGUEBATTLESTARTMATCHNTF_H__
#define __PTCM2C_LEAGUEBATTLESTARTMATCHNTF_H__

// generate by ProtoGen at date: 2017/1/6 15:25:40

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2C_LEAGUEBATTLESTARTMATCHNTF_TYPE 61870

class PtcM2C_LeagueBattleStartMatchNtf : public CProtocol
{
public:
    explicit PtcM2C_LeagueBattleStartMatchNtf() : CProtocol(PTCM2C_LEAGUEBATTLESTARTMATCHNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2C_LeagueBattleStartMatchNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::LeagueBattleStartMatchNtf m_Data;
};

#endif