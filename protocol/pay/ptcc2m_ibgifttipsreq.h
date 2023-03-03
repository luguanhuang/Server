#ifndef __PTCC2M_IBGIFTTIPSREQ_H__
#define __PTCC2M_IBGIFTTIPSREQ_H__

// generate by ProtoGen at date: 2017/3/20 19:59:52

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCC2M_IBGIFTTIPSREQ_TYPE 29090

class PtcC2M_IBGiftTipsReq : public CProtocol
{
public:
    explicit PtcC2M_IBGiftTipsReq() : CProtocol(PTCC2M_IBGIFTTIPSREQ_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcC2M_IBGiftTipsReq()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::IBGiftTips m_Data;
};

#endif