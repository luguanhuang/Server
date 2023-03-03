#ifndef __PTCG2C_CHANGEFASHIONNOTIFY_H__
#define __PTCG2C_CHANGEFASHIONNOTIFY_H__

// generate by ProtoGen at date: 2015/6/16 15:59:12

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_CHANGEFASHIONNOTIFY_TYPE 1731

class PtcG2C_ChangeFashionNotify : public CProtocol
{
public:
    explicit PtcG2C_ChangeFashionNotify() : CProtocol(PTCG2C_CHANGEFASHIONNOTIFY_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_ChangeFashionNotify()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::FashionChanged m_Data;
};

#endif