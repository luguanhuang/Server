#ifndef __PTCK2M_BMCROSSNTFDATANTF_H__
#define __PTCK2M_BMCROSSNTFDATANTF_H__

// generate by ProtoGen at date: 2017/6/22 8:19:05

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCK2M_BMCROSSNTFDATANTF_TYPE 38469

class PtcK2M_BMCrossNtfDataNtf : public CProtocol
{
public:
    explicit PtcK2M_BMCrossNtfDataNtf() : CProtocol(PTCK2M_BMCROSSNTFDATANTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcK2M_BMCrossNtfDataNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::BMCrossNtfData m_Data;
};

#endif