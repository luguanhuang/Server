#ifndef __XCOMPONENT_H__
#define __XCOMPONENT_H__

#include "XComponentDefine.hpp"
#include "entity/XActionDefine.h"

using namespace std;

class XObject;
class XEntity;
class XActionArgs;

typedef XDelegate XActionHandler;

class XComponent
{
	friend class XObject;

public:
	XComponent();
	virtual ~XComponent();

	virtual UINT32 ID() = 0;
	inline bool IsEnabled(){ return _enabled; }
	inline void Enable(bool enable){ _enabled = enable; }
	inline XEntity* GetEntity() { return _pEntity; }

protected:
	virtual void OnAttachToHost(XObject* pHost);
	virtual void OnDetachFromHost();
	virtual void OnOtherComponentAttachToHost(XComponent* pc) {};
	virtual void OnOtherComponentDetachFromHost(XComponent* pc) {};
	virtual void Attached(){};
	virtual void Update(float fDeltaT) {};

	bool OnReceiveAction(XActionArgs* args);
	virtual void ActionSubscribe() {};
	void ActionUnsubscribe();

	void RegisterAction(XActionDefine action, const XActionHandler& handler);

	inline XObject* GetHost() { return _pObj; }

protected:
	UINT64 _object_id;

private:
	XObject* _pObj;
	XEntity* _pEntity;

	bool _enabled;

	XActionHandler* _actionMap[XAction_Max];
};

#endif // __XCOMPONENT_H__