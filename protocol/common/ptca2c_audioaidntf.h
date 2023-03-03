#ifndef __PTCA2C_AUDIOAIDNTF_H__
#define __PTCA2C_AUDIOAIDNTF_H__

// generate by ProtoGen at date: 2017/3/4 23:21:49

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCA2C_AUDIOAIDNTF_TYPE 54517

class PtcA2C_AudioAIDNtf : public CProtocol
{
public:
    explicit PtcA2C_AudioAIDNtf() : CProtocol(PTCA2C_AUDIOAIDNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcA2C_AudioAIDNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::AudioTextArg m_Data;
};

#endif