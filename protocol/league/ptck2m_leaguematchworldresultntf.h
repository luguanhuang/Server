#ifndef __PTCK2M_LEAGUEMATCHWORLDRESULTNTF_H__
#define __PTCK2M_LEAGUEMATCHWORLDRESULTNTF_H__

// generate by ProtoGen at date: 2017/2/3 15:18:49

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCK2M_LEAGUEMATCHWORLDRESULTNTF_TYPE 64974

class PtcK2M_LeagueMatchWorldResultNtf : public CProtocol
{
public:
    explicit PtcK2M_LeagueMatchWorldResultNtf() : CProtocol(PTCK2M_LEAGUEMATCHWORLDRESULTNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcK2M_LeagueMatchWorldResultNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::LeagueMatchWorldResultNtf m_Data;
};

#endif