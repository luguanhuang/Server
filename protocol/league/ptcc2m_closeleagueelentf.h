#ifndef __PTCC2M_CLOSELEAGUEELENTF_H__
#define __PTCC2M_CLOSELEAGUEELENTF_H__

// generate by ProtoGen at date: 2017/1/14 21:42:09

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCC2M_CLOSELEAGUEELENTF_TYPE 8195

class PtcC2M_CloseLeagueEleNtf : public CProtocol
{
public:
    explicit PtcC2M_CloseLeagueEleNtf() : CProtocol(PTCC2M_CLOSELEAGUEELENTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcC2M_CloseLeagueEleNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::CloseLeagueEleNtf m_Data;
};

#endif