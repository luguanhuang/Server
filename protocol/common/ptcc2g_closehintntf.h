#ifndef __PTCC2G_CLOSEHINTNTF_H__
#define __PTCC2G_CLOSEHINTNTF_H__

// generate by ProtoGen at date: 2017/4/11 16:08:34

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCC2G_CLOSEHINTNTF_TYPE 37802

class PtcC2G_CloseHintNtf : public CProtocol
{
public:
    explicit PtcC2G_CloseHintNtf() : CProtocol(PTCC2G_CLOSEHINTNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcC2G_CloseHintNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::CloseHintNtf m_Data;
};

#endif