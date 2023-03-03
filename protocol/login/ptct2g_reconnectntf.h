#ifndef __PTCT2G_RECONNECTNTF_H__
#define __PTCT2G_RECONNECTNTF_H__

// generate by ProtoGen at date: 2016/1/18 10:44:42

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCT2G_RECONNECTNTF_TYPE 50526

class PtcT2G_ReconnectNtf : public CProtocol
{
public:
    explicit PtcT2G_ReconnectNtf() : CProtocol(PTCT2G_RECONNECTNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcT2G_ReconnectNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::ReconnectNtf m_Data;
};

#endif