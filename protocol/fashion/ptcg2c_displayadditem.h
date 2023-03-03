#ifndef __PTCG2C_DISPLAYADDITEM_H__
#define __PTCG2C_DISPLAYADDITEM_H__

// generate by ProtoGen at date: 2017/5/3 11:47:08

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_DISPLAYADDITEM_TYPE 55159

class PtcG2C_DisplayAddItem : public CProtocol
{
public:
    explicit PtcG2C_DisplayAddItem() : CProtocol(PTCG2C_DISPLAYADDITEM_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_DisplayAddItem()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::DisplayAddItemArg m_Data;
};

#endif