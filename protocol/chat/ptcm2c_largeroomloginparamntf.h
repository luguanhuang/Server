#ifndef __PTCM2C_LARGEROOMLOGINPARAMNTF_H__
#define __PTCM2C_LARGEROOMLOGINPARAMNTF_H__

// generate by ProtoGen at date: 2016/11/24 20:44:07

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2C_LARGEROOMLOGINPARAMNTF_TYPE 51856

class PtcM2C_LargeRoomLoginParamNtf : public CProtocol
{
public:
    explicit PtcM2C_LargeRoomLoginParamNtf() : CProtocol(PTCM2C_LARGEROOMLOGINPARAMNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2C_LargeRoomLoginParamNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::LargeRoomLoginParam m_Data;
};

#endif