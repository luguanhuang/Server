#ifndef __PTCM2C_DRAGONGUILDSETTINGCHANGED_H__
#define __PTCM2C_DRAGONGUILDSETTINGCHANGED_H__

// generate by ProtoGen at date: 2017/9/7 16:29:48

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2C_DRAGONGUILDSETTINGCHANGED_TYPE 42603

class PtcM2C_DragonGuildSettingChanged : public CProtocol
{
public:
    explicit PtcM2C_DragonGuildSettingChanged() : CProtocol(PTCM2C_DRAGONGUILDSETTINGCHANGED_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2C_DragonGuildSettingChanged()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::DragonGuildSettingChanged m_Data;
};

#endif