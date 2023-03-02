#ifndef __XGLOBALAI_OBJECT_H__
#define __XGLOBALAI_OBJECT_H__

#include "XEntity.h"

class XComponent;

class XGlobalAIObject : public XEntity
{
	friend class GlobalAIUnit;

public:
	XGlobalAIObject(Unit* pUnit);
	virtual ~XGlobalAIObject();

public:
	//override the Initilize
	virtual bool Initilize();
	virtual void Uninitilize();
	virtual void OnCreated();

};

#endif