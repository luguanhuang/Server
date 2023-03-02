#ifndef __XALLY_H__
#define __XALLY_H__

#include "XEntity.h"

class XComponent;

class XDummyRole : public XEntity
{
	friend class DummyRole;

protected:
	XDummyRole(Unit* pUnit);
	virtual ~XDummyRole();

	//override the Initilize
	virtual bool Initilize();
};

#endif