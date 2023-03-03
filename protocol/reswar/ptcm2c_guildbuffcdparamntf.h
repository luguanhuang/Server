#ifndef __PTCM2C_GUILDBUFFCDPARAMNTF_H__
#define __PTCM2C_GUILDBUFFCDPARAMNTF_H__

// generate by ProtoGen at date: 2016/11/30 14:56:03

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2C_GUILDBUFFCDPARAMNTF_TYPE 4703

class PtcM2C_GuildBuffCDParamNtf : public CProtocol
{
public:
    explicit PtcM2C_GuildBuffCDParamNtf() : CProtocol(PTCM2C_GUILDBUFFCDPARAMNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2C_GuildBuffCDParamNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::GuildBuffCDParam m_Data;
};

#endif