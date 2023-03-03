#ifndef __PTCT2C_CHATNOTIFY_H__
#define __PTCT2C_CHATNOTIFY_H__

// generate by ProtoGen at date: 2016/6/25 15:42:23

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCT2C_CHATNOTIFY_TYPE 4256

class PtcT2C_ChatNotify : public CProtocol
{
public:
    explicit PtcT2C_ChatNotify() : CProtocol(PTCT2C_CHATNOTIFY_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcT2C_ChatNotify()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::ChatNotify m_Data;
};

#endif