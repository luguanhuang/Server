#ifndef __PTCN2T_SESSIONCHANGE2GT_H__
#define __PTCN2T_SESSIONCHANGE2GT_H__

// generate by ProtoGen at date: 2017/6/21 17:36:18

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCN2T_SESSIONCHANGE2GT_TYPE 29446

class PtcN2T_SessionChange2Gt : public CProtocol
{
public:
    explicit PtcN2T_SessionChange2Gt() : CProtocol(PTCN2T_SESSIONCHANGE2GT_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcN2T_SessionChange2Gt()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::SessionChangeData m_Data;
};

#endif