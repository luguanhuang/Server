#ifndef __PTCG2C_BUFFNOTIFY_H__
#define __PTCG2C_BUFFNOTIFY_H__

// generate by ProtoGen at date: 2015/11/26 21:23:49

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_BUFFNOTIFY_TYPE 18520

class PtcG2C_BuffNotify : public CProtocol
{
public:
    explicit PtcG2C_BuffNotify() : CProtocol(PTCG2C_BUFFNOTIFY_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_BuffNotify()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::buffInfo m_Data;
};

#endif