#ifndef __PTCF2M_LARGEROOMRADIONTF_H__
#define __PTCF2M_LARGEROOMRADIONTF_H__

// generate by ProtoGen at date: 2016/11/25 10:00:06

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCF2M_LARGEROOMRADIONTF_TYPE 38425

class PtcF2M_LargeRoomRadioNtf : public CProtocol
{
public:
    explicit PtcF2M_LargeRoomRadioNtf() : CProtocol(PTCF2M_LARGEROOMRADIONTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcF2M_LargeRoomRadioNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::LargeRoomRoleParam m_Data;
};

#endif