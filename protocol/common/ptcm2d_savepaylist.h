#ifndef __PTCM2D_SAVEPAYLIST_H__
#define __PTCM2D_SAVEPAYLIST_H__

// generate by ProtoGen at date: 2016/10/27 22:06:35

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2D_SAVEPAYLIST_TYPE 20269

class PtcM2D_SavePayList : public CProtocol
{
public:
    explicit PtcM2D_SavePayList() : CProtocol(PTCM2D_SAVEPAYLIST_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2D_SavePayList()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::PayList m_Data;
};

#endif