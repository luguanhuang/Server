#ifndef __PTCC2M_MULACTIVITYICONSYSREQ_H__
#define __PTCC2M_MULACTIVITYICONSYSREQ_H__

// generate by ProtoGen at date: 2017/10/7 15:44:50

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCC2M_MULACTIVITYICONSYSREQ_TYPE 64642

class PtcC2M_MulActivityIconSysReq : public CProtocol
{
public:
    explicit PtcC2M_MulActivityIconSysReq() : CProtocol(PTCC2M_MULACTIVITYICONSYSREQ_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcC2M_MulActivityIconSysReq()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::MulActivityIconSys m_Data;
};

#endif