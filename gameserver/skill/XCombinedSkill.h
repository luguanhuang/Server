#ifndef __XCOMBINEDSKILL_H__
#define __XCOMBINEDSKILL_H__

#include "XArtsSkill.h"

class XEntity;
class XSkillCore;

class XCombinedSkill : public XArtsSkill
{
public:
	XCombinedSkill(XEntity* firer);
	virtual ~XCombinedSkill();

	virtual XSkillCore* MainCore() const { return _main_core; }

	void CombinedStop();
	void CombinedKillTimerAll();

	void ShutDown();

	inline virtual UINT32 GetCombinedId() const { return _combined_id; }
	inline void AddedCombinedTimerToken(HTIMER token) { _combined_actionToken.push_back(token); }

protected:
	virtual bool Launch(XSkillCore* next);
	virtual void FireEvents();

private:
	void MainCoreStart();
	bool Combined(IArgs* args, void*);

private:
	XSkillCore* _main_core;

	int _result_time;

	UINT32 _combined_id;
	HTIMER _combined_token;

	bool _shut_down;

	vector<HTIMER> _combined_actionToken;
};

#endif	//__XCOMBINEDSKILL_H__