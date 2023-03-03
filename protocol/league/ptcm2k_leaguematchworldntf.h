#ifndef __PTCM2K_LEAGUEMATCHWORLDNTF_H__
#define __PTCM2K_LEAGUEMATCHWORLDNTF_H__

// generate by ProtoGen at date: 2017/2/3 15:11:27

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2K_LEAGUEMATCHWORLDNTF_TYPE 39077

class PtcM2K_LeagueMatchWorldNtf : public CProtocol
{
public:
    explicit PtcM2K_LeagueMatchWorldNtf() : CProtocol(PTCM2K_LEAGUEMATCHWORLDNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2K_LeagueMatchWorldNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::LeagueMatchWorldNtf m_Data;
};

#endif