#ifndef __PTCM2G_CONTROLFUNCTIONNTF_H__
#define __PTCM2G_CONTROLFUNCTIONNTF_H__

// generate by ProtoGen at date: 2017/1/18 21:06:34

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2G_CONTROLFUNCTIONNTF_TYPE 3181

class PtcM2G_ControlFunctionNtf : public CProtocol
{
public:
    explicit PtcM2G_ControlFunctionNtf() : CProtocol(PTCM2G_CONTROLFUNCTIONNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2G_ControlFunctionNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::ControlFunciontInfo m_Data;
};

#endif