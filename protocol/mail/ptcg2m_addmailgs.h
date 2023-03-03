#ifndef __PTCG2M_ADDMAILGS_H__
#define __PTCG2M_ADDMAILGS_H__

// generate by ProtoGen at date: 2016/7/31 22:49:48

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2M_ADDMAILGS_TYPE 44628

class PtcG2M_AddMailGS : public CProtocol
{
public:
    explicit PtcG2M_AddMailGS() : CProtocol(PTCG2M_ADDMAILGS_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2M_AddMailGS()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::AddMailGS m_Data;
};

#endif