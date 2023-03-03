#ifndef __PTCG2M_OPENSYSTEMNTFMS_H__
#define __PTCG2M_OPENSYSTEMNTFMS_H__

// generate by ProtoGen at date: 2017/2/9 20:15:45

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2M_OPENSYSTEMNTFMS_TYPE 53016

class PtcG2M_OpenSystemNtfMs : public CProtocol
{
public:
    explicit PtcG2M_OpenSystemNtfMs() : CProtocol(PTCG2M_OPENSYSTEMNTFMS_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2M_OpenSystemNtfMs()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::OpenSystemNtfMsData m_Data;
};

#endif