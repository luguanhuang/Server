#ifndef __PTCG2M_RESWARTEAMRESULTNTF_H__
#define __PTCG2M_RESWARTEAMRESULTNTF_H__

// generate by ProtoGen at date: 2016/11/1 14:22:27

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2M_RESWARTEAMRESULTNTF_TYPE 44866

class PtcG2M_ResWarTeamResultNtf : public CProtocol
{
public:
    explicit PtcG2M_ResWarTeamResultNtf() : CProtocol(PTCG2M_RESWARTEAMRESULTNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2M_ResWarTeamResultNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::ResWarTeamResult m_Data;
};

#endif