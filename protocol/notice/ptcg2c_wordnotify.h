#ifndef __PTCG2C_WORDNOTIFY_H__
#define __PTCG2C_WORDNOTIFY_H__

// generate by ProtoGen at date: 2017/10/9 16:22:33

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_WORDNOTIFY_TYPE 34052

class PtcG2C_WordNotify : public CProtocol
{
public:
    explicit PtcG2C_WordNotify() : CProtocol(PTCG2C_WORDNOTIFY_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_WordNotify()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::WordNotify m_Data;
};

#endif