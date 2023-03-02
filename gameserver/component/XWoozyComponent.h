#ifndef __XWOOZYCOMPONENT_H__
#define __XWOOZYCOMPONENT_H__

#include "XComponent.h"

class XWoozyComponent : public XComponent
{
public:

	static const UINT32 uuID = XWoozy_Component;

	XWoozyComponent();
	~XWoozyComponent();

	virtual UINT32 ID() { return XWoozyComponent::uuID; }
	virtual void Attached();
	virtual void Update(float fDeltaT);
	virtual void OnDetachFromHost();

	inline bool InTransfer() const { return _transfer; }

protected:
	virtual void ActionSubscribe();

private:
	bool OnDead(IArgs* pargs, void *);
	bool OnArmorBroken(IArgs* pargs, void *);
	bool OnArmorRecover(IArgs* pargs, void *);

	void Clear();

	void BrokenArmor();
	void RecoverArmor();
	void Woozy();

	bool WoozyOn(IArgs* pargs, void *);
	bool WoozyOff(IArgs* pargs, void *);

private:
	bool _OnBroken;
	bool _OnRecover;
	bool _OnWoozy;

	bool _transfer;

	bool _woozy_enabled;

	HTIMER _token_on;
	HTIMER _token_off;
};

#endif	//__XWOOZYCOMPONENT_H__