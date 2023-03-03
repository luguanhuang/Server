#ifndef __PTCC2M_CLOSESKYCRAFTELINTF_H__
#define __PTCC2M_CLOSESKYCRAFTELINTF_H__

// generate by ProtoGen at date: 2017/4/11 17:59:34

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCC2M_CLOSESKYCRAFTELINTF_TYPE 46239

class PtcC2M_CloseSkyCraftEliNtf : public CProtocol
{
public:
    explicit PtcC2M_CloseSkyCraftEliNtf() : CProtocol(PTCC2M_CLOSESKYCRAFTELINTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcC2M_CloseSkyCraftEliNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::CloseSkyCraftEliNtf m_Data;
};

#endif