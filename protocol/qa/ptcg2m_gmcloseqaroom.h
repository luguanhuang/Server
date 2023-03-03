#ifndef __PTCG2M_GMCLOSEQAROOM_H__
#define __PTCG2M_GMCLOSEQAROOM_H__

// generate by ProtoGen at date: 2016/10/9 19:47:35

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2M_GMCLOSEQAROOM_TYPE 22741

class PtcG2M_GMCloseQARoom : public CProtocol
{
public:
    explicit PtcG2M_GMCloseQARoom() : CProtocol(PTCG2M_GMCLOSEQAROOM_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2M_GMCloseQARoom()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::GMCloseQARoom m_Data;
};

#endif