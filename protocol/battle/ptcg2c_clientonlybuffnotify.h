#ifndef __PTCG2C_CLIENTONLYBUFFNOTIFY_H__
#define __PTCG2C_CLIENTONLYBUFFNOTIFY_H__

// generate by ProtoGen at date: 2016/7/27 21:26:02

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_CLIENTONLYBUFFNOTIFY_TYPE 35149

class PtcG2C_ClientOnlyBuffNotify : public CProtocol
{
public:
    explicit PtcG2C_ClientOnlyBuffNotify() : CProtocol(PTCG2C_CLIENTONLYBUFFNOTIFY_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_ClientOnlyBuffNotify()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::BuffList m_Data;
};

#endif