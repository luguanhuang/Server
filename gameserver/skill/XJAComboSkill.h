#ifndef __XJACOMBOSKILL_H__
#define __XJACOMBOSKILL_H__

#include "XArtsSkill.h"

class XEntity;

class XJAComboSkill : public XArtsSkill
{
public:
	XJAComboSkill(XEntity* firer);
	virtual ~XJAComboSkill();

	bool ReFire(IArgs* args, void*);
	inline bool DuringJA(){ return _during_ja; }

protected:
	//skill do self starting here
	virtual void Start();
	//skill do self stoppage here
	virtual void Stop();

private:
	void JAAt();
	bool ValidJA();

	bool Refire(IArgs* args, void*);
	UINT32 GetNextJAIdentify();

private:
	bool _during_ja;
};

#endif	//__XJACOMBOSKILL_H__