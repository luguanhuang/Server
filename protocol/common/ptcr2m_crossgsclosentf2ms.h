#ifndef __PTCR2M_CROSSGSCLOSENTF2MS_H__
#define __PTCR2M_CROSSGSCLOSENTF2MS_H__

// generate by ProtoGen at date: 2016/11/28 17:37:50

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCR2M_CROSSGSCLOSENTF2MS_TYPE 45650

class PtcR2M_CrossGsCloseNtf2Ms : public CProtocol
{
public:
    explicit PtcR2M_CrossGsCloseNtf2Ms() : CProtocol(PTCR2M_CROSSGSCLOSENTF2MS_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcR2M_CrossGsCloseNtf2Ms()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::CrossGsCloseNtf2Ms m_Data;
};

#endif