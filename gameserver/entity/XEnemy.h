#ifndef __XENEMY_H__
#define __XENEMY_H__

#include "XEntity.h"

class XComponent;

class XEnemy : public XEntity
{
	friend class Enemy;

protected:
	XEnemy(Unit* pUnit);
	virtual ~XEnemy();

	//override the Initilize
	virtual bool Initilize();
};

#endif