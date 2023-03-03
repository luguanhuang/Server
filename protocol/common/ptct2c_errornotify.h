#ifndef __PTCT2C_ERRORNOTIFY_H__
#define __PTCT2C_ERRORNOTIFY_H__

// generate by ProtoGen at date: 2016/10/28 22:22:56

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCT2C_ERRORNOTIFY_TYPE 21940

class PtcT2C_ErrorNotify : public CProtocol
{
public:
    explicit PtcT2C_ErrorNotify() : CProtocol(PTCT2C_ERRORNOTIFY_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcT2C_ErrorNotify()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::ErrorInfo m_Data;
};

#endif