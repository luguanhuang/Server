#ifndef __PTCC2M_GOBACKREADYSCENENTF_H__
#define __PTCC2M_GOBACKREADYSCENENTF_H__

// generate by ProtoGen at date: 2017/9/1 15:43:50

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCC2M_GOBACKREADYSCENENTF_TYPE 10491

class PtcC2M_GoBackReadySceneNtf : public CProtocol
{
public:
    explicit PtcC2M_GoBackReadySceneNtf() : CProtocol(PTCC2M_GOBACKREADYSCENENTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcC2M_GoBackReadySceneNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::GoBackReadyScene m_Data;
};

#endif