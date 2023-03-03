#ifndef __PTCN2G_UPDATEFUNCTIONOPEN2GS_H__
#define __PTCN2G_UPDATEFUNCTIONOPEN2GS_H__

// generate by ProtoGen at date: 2016/11/21 16:50:14

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCN2G_UPDATEFUNCTIONOPEN2GS_TYPE 61771

class PtcN2G_UpdateFunctionOpen2Gs : public CProtocol
{
public:
    explicit PtcN2G_UpdateFunctionOpen2Gs() : CProtocol(PTCN2G_UPDATEFUNCTIONOPEN2GS_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcN2G_UpdateFunctionOpen2Gs()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::FunctionOpenInfo m_Data;
};

#endif