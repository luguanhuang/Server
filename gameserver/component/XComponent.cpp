#include "pch.h"
#include "XComponent.h"
#include "entity/XObject.h"
#include "entity/XEntity.h"

XComponent::XComponent()
	:_object_id(0), _pObj(NULL), _pEntity(NULL), _enabled(true)
{
	memset(_actionMap, NULL, sizeof(XActionHandler*) * XAction_Max);
}

XComponent::~XComponent()
{
	for(int i = 0; i < XAction_Max; i++)
	{
		if(!_actionMap[i]) continue;

		delete _actionMap[i];
		_actionMap[i] = NULL;
	}
}

void XComponent::OnAttachToHost(XObject* pHost)
{
	_object_id = pHost->GetID();
	_pObj = pHost;

	_pEntity = dynamic_cast<XEntity*>(_pObj);

	ActionSubscribe();
}

void XComponent::OnDetachFromHost()
{
	ActionUnsubscribe();

	_object_id = 0;
	_pObj = 0;
	_pEntity = 0;
}

void XComponent::ActionUnsubscribe()
{
	XObject* pObj = GetHost();

	for(int i = 0; i < XAction_Max; i++)
	{
		if(!_actionMap[i]) continue;

		if(pObj) pObj->DetachAction((XActionDefine)i, ID());

		delete _actionMap[i];
		_actionMap[i] = NULL;
	}
}

// the later event handler will override the earlier.
void XComponent::RegisterAction(XActionDefine action, const XActionHandler& handler)
{
	if(action < 0 || action >= XAction_Max) return;

	XObject* pObj = GetHost();
	if(pObj) pObj->AttachAction(action, ID());

	if(_actionMap[action])
	{
		LogError("Fatal: Handler for Action %d has been registered before in Component %d, the older handler will be over-written.", action, ID());
	}
	else
	{
		_actionMap[action] = new XActionHandler;
	}

	*_actionMap[action] = handler;
}

bool XComponent::OnReceiveAction(XActionArgs* args)
{
	int def = args->ArgsDefine();
	if(def < 0 || def >= XAction_Max) return false;

	return _actionMap[def] ? (*(_actionMap[def]))(args, NULL) : false;
}