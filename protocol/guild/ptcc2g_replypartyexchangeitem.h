#ifndef __PTCC2G_REPLYPARTYEXCHANGEITEM_H__
#define __PTCC2G_REPLYPARTYEXCHANGEITEM_H__

// generate by ProtoGen at date: 2017/5/2 15:42:16

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCC2G_REPLYPARTYEXCHANGEITEM_TYPE 62563

class PtcC2G_ReplyPartyExchangeItem : public CProtocol
{
public:
    explicit PtcC2G_ReplyPartyExchangeItem() : CProtocol(PTCC2G_REPLYPARTYEXCHANGEITEM_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcC2G_ReplyPartyExchangeItem()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::ReplyPartyExchangeItemArg m_Data;
};

#endif