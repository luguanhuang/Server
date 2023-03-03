#ifndef __PTCK2G_NEWROUTERNTF_H__
#define __PTCK2G_NEWROUTERNTF_H__

// generate by ProtoGen at date: 2016/10/27 20:10:23

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCK2G_NEWROUTERNTF_TYPE 16492

class PtcK2G_NewRouterNtf : public CProtocol
{
public:
    explicit PtcK2G_NewRouterNtf() : CProtocol(PTCK2G_NEWROUTERNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcK2G_NewRouterNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::NewRouterInfo m_Data;
};

#endif