#ifndef __PTCM2C_MSERRORNOTIFY_H__
#define __PTCM2C_MSERRORNOTIFY_H__

// generate by ProtoGen at date: 2016/8/18 14:17:41

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2C_MSERRORNOTIFY_TYPE 48740

class PtcM2C_MSErrorNotify : public CProtocol
{
public:
    explicit PtcM2C_MSErrorNotify() : CProtocol(PTCM2C_MSERRORNOTIFY_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2C_MSErrorNotify()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::ErrorInfo m_Data;
};

#endif