#ifndef __PTCM2C_IDIPPUNISHINFOMSNTF_H__
#define __PTCM2C_IDIPPUNISHINFOMSNTF_H__

// generate by ProtoGen at date: 2016/9/29 20:22:59

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2C_IDIPPUNISHINFOMSNTF_TYPE 8208

class PtcM2C_IdipPunishInfoMsNtf : public CProtocol
{
public:
    explicit PtcM2C_IdipPunishInfoMsNtf() : CProtocol(PTCM2C_IDIPPUNISHINFOMSNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2C_IdipPunishInfoMsNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::IdipPunishInfo m_Data;
};

#endif