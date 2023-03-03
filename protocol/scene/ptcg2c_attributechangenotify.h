#ifndef __PTCG2C_ATTRIBUTECHANGENOTIFY_H__
#define __PTCG2C_ATTRIBUTECHANGENOTIFY_H__

// generate by ProtoGen at date: 2014/12/29 17:54:19

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_ATTRIBUTECHANGENOTIFY_TYPE 57626

class PtcG2C_AttributeChangeNotify : public CProtocol
{
public:
    explicit PtcG2C_AttributeChangeNotify() : CProtocol(PTCG2C_ATTRIBUTECHANGENOTIFY_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_AttributeChangeNotify()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::ChangedAttribute m_Data;
};

#endif