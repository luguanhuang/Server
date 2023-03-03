#ifndef __PTCG2M_SKYCITYTEAMRESULTNTF_H__
#define __PTCG2M_SKYCITYTEAMRESULTNTF_H__

// generate by ProtoGen at date: 2016/10/8 17:04:39

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2M_SKYCITYTEAMRESULTNTF_TYPE 46536

class PtcG2M_SkyCityTeamResultNtf : public CProtocol
{
public:
    explicit PtcG2M_SkyCityTeamResultNtf() : CProtocol(PTCG2M_SKYCITYTEAMRESULTNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2M_SkyCityTeamResultNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::SkyCityTeamResult m_Data;
};

#endif