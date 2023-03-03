#ifndef __PTCG2C_HINTNOTIFY_H__
#define __PTCG2C_HINTNOTIFY_H__

// generate by ProtoGen at date: 2017/3/15 10:57:44

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_HINTNOTIFY_TYPE 23114

class PtcG2C_HintNotify : public CProtocol
{
public:
    explicit PtcG2C_HintNotify() : CProtocol(PTCG2C_HINTNOTIFY_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_HintNotify()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::HintNotify m_Data;
};

#endif