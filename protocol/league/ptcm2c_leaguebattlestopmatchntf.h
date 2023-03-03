#ifndef __PTCM2C_LEAGUEBATTLESTOPMATCHNTF_H__
#define __PTCM2C_LEAGUEBATTLESTOPMATCHNTF_H__

// generate by ProtoGen at date: 2017/6/6 18:05:19

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2C_LEAGUEBATTLESTOPMATCHNTF_TYPE 53912

class PtcM2C_LeagueBattleStopMatchNtf : public CProtocol
{
public:
    explicit PtcM2C_LeagueBattleStopMatchNtf() : CProtocol(PTCM2C_LEAGUEBATTLESTOPMATCHNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2C_LeagueBattleStopMatchNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::LeagueBattleStopMatchNtf m_Data;
};

#endif