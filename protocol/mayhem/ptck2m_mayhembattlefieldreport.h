#ifndef __PTCK2M_MAYHEMBATTLEFIELDREPORT_H__
#define __PTCK2M_MAYHEMBATTLEFIELDREPORT_H__

// generate by ProtoGen at date: 2017/6/22 8:19:25

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCK2M_MAYHEMBATTLEFIELDREPORT_TYPE 43066

class PtcK2M_MayhemBattlefieldReport : public CProtocol
{
public:
    explicit PtcK2M_MayhemBattlefieldReport() : CProtocol(PTCK2M_MAYHEMBATTLEFIELDREPORT_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcK2M_MayhemBattlefieldReport()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::MayhemBattlefieldArg m_Data;
};

#endif