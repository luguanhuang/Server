#ifndef __PTCM2C_UPDATELEAGUEELEROOMSTATENTF_H__
#define __PTCM2C_UPDATELEAGUEELEROOMSTATENTF_H__

// generate by ProtoGen at date: 2017/1/14 15:52:11

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2C_UPDATELEAGUEELEROOMSTATENTF_TYPE 15800

class PtcM2C_UpdateLeagueEleRoomStateNtf : public CProtocol
{
public:
    explicit PtcM2C_UpdateLeagueEleRoomStateNtf() : CProtocol(PTCM2C_UPDATELEAGUEELEROOMSTATENTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2C_UpdateLeagueEleRoomStateNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::UpdateLeagueEleRoomStateNtf m_Data;
};

#endif