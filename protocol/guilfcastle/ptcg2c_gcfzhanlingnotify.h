#ifndef __PTCG2C_GCFZHANLINGNOTIFY_H__
#define __PTCG2C_GCFZHANLINGNOTIFY_H__

// generate by ProtoGen at date: 2016/12/14 23:25:15

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_GCFZHANLINGNOTIFY_TYPE 14402

class PtcG2C_GCFZhanLingNotify : public CProtocol
{
public:
    explicit PtcG2C_GCFZhanLingNotify() : CProtocol(PTCG2C_GCFZHANLINGNOTIFY_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_GCFZhanLingNotify()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::GCFZhanLingPara m_Data;
};

#endif