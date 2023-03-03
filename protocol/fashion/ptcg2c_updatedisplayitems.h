#ifndef __PTCG2C_UPDATEDISPLAYITEMS_H__
#define __PTCG2C_UPDATEDISPLAYITEMS_H__

// generate by ProtoGen at date: 2017/5/4 11:44:22

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_UPDATEDISPLAYITEMS_TYPE 12217

class PtcG2C_UpdateDisplayItems : public CProtocol
{
public:
    explicit PtcG2C_UpdateDisplayItems() : CProtocol(PTCG2C_UPDATEDISPLAYITEMS_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_UpdateDisplayItems()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::UpdateDisplayItems m_Data;
};

#endif