#ifndef __PTCM2C_LEAGUEBATTLEMATCHTIMEOUTNTF_H__
#define __PTCM2C_LEAGUEBATTLEMATCHTIMEOUTNTF_H__

// generate by ProtoGen at date: 2017/1/11 16:34:17

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2C_LEAGUEBATTLEMATCHTIMEOUTNTF_TYPE 31012

class PtcM2C_LeagueBattleMatchTimeoutNtf : public CProtocol
{
public:
    explicit PtcM2C_LeagueBattleMatchTimeoutNtf() : CProtocol(PTCM2C_LEAGUEBATTLEMATCHTIMEOUTNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2C_LeagueBattleMatchTimeoutNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::LeagueBattleMatchTimeoutNtf m_Data;
};

#endif