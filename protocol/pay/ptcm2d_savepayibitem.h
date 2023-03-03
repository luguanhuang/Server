#ifndef __PTCM2D_SAVEPAYIBITEM_H__
#define __PTCM2D_SAVEPAYIBITEM_H__

// generate by ProtoGen at date: 2017/3/23 11:03:55

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2D_SAVEPAYIBITEM_TYPE 53614

class PtcM2D_SavePayIbItem : public CProtocol
{
public:
    explicit PtcM2D_SavePayIbItem() : CProtocol(PTCM2D_SAVEPAYIBITEM_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2D_SavePayIbItem()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::SavePayIbItemNtf m_Data;
};

#endif