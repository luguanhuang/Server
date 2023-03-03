#ifndef __PTCN2M_UPDATEFUNCTIONOPEN2MS_H__
#define __PTCN2M_UPDATEFUNCTIONOPEN2MS_H__

// generate by ProtoGen at date: 2016/11/21 16:49:45

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCN2M_UPDATEFUNCTIONOPEN2MS_TYPE 7109

class PtcN2M_UpdateFunctionOpen2Ms : public CProtocol
{
public:
    explicit PtcN2M_UpdateFunctionOpen2Ms() : CProtocol(PTCN2M_UPDATEFUNCTIONOPEN2MS_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcN2M_UpdateFunctionOpen2Ms()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::FunctionOpenInfo m_Data;
};

#endif