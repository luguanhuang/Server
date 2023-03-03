#ifndef __PTCM2C_UPDATELEAGUEBATTLESEASONINFO_H__
#define __PTCM2C_UPDATELEAGUEBATTLESEASONINFO_H__

// generate by ProtoGen at date: 2017/1/5 22:58:02

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2C_UPDATELEAGUEBATTLESEASONINFO_TYPE 42828

class PtcM2C_UpdateLeagueBattleSeasonInfo : public CProtocol
{
public:
    explicit PtcM2C_UpdateLeagueBattleSeasonInfo() : CProtocol(PTCM2C_UPDATELEAGUEBATTLESEASONINFO_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2C_UpdateLeagueBattleSeasonInfo()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::UpdateLeagueBattleSeasonInfo m_Data;
};

#endif