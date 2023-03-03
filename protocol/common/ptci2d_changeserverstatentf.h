#ifndef __PTCI2D_CHANGESERVERSTATENTF_H__
#define __PTCI2D_CHANGESERVERSTATENTF_H__

// generate by ProtoGen at date: 2016/3/3 20:29:49

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCI2D_CHANGESERVERSTATENTF_TYPE 28511

class PtcI2D_ChangeServerStateNtf : public CProtocol
{
public:
    explicit PtcI2D_ChangeServerStateNtf() : CProtocol(PTCI2D_CHANGESERVERSTATENTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcI2D_ChangeServerStateNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::ServerListArg m_Data;
};

#endif