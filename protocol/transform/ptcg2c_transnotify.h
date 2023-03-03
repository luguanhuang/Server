#ifndef __PTCG2C_TRANSNOTIFY_H__
#define __PTCG2C_TRANSNOTIFY_H__

// generate by ProtoGen at date: 2017/9/18 14:21:36

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_TRANSNOTIFY_TYPE 15935

class PtcG2C_TransNotify : public CProtocol
{
public:
    explicit PtcG2C_TransNotify() : CProtocol(PTCG2C_TRANSNOTIFY_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_TransNotify()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::TransNotify m_Data;
};

#endif