#ifndef __XARTSSKILL_H__
#define __XARTSSKILL_H__

#include "XSkill.h"

struct XResultData;

class XEntity;
class XBullet;
class IArgs;

class XArtsSkill : public XSkill
{
public:
	XArtsSkill(XEntity* firer);
	virtual ~XArtsSkill();

//protected:
public:
	//message func
	bool Project(XResultData* data, int additionalAngle = 0, int loop = 0, int group = 0);
	bool GroupResults(IArgs* args, void*);
	bool LoopResults(IArgs* args, void*);

	//skill do self starting here
	virtual void Start();
	//skill do self stoppage here
	virtual void Stop();
	//update effects per frame
	virtual bool Present(float fDeltaT);
	//set skill result point here
	virtual void Result(XResultData* data);

	XBullet* GenerateBullet(XResultData* data, XEntity* target, int additional, int loop, int group, int wid = -1, bool extrainfo = false);

	bool QTEOn(IArgs*, void* param);
	bool QTEOff(IArgs*, void* param);

	bool ChargeTo(IArgs* args, void*);
	bool Manipulate(IArgs* args, void*);
	bool KillManipulate(IArgs* args, void*);
	bool Warning(IArgs*, void* param);
	bool PreservedS(IArgs*, void* param);
	bool ExString(IArgs*, void* param);
	bool Mob(IArgs*, void* param);
	bool CastChain(IArgs*, void* param);

protected:
	virtual void FireEvents();

private:
	vector<UINT64> _mob_unit;
};

#endif	//__XARTSSKILL_H__
