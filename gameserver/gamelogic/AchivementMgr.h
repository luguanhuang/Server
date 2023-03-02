#ifndef __ACHIVEMENTMGR_H__
#define __ACHIVEMENTMGR_H__

#include "table/AchivementTable.h"
#include "AchivementDef.h"

class IAchivementHandler;

class AchivementMgr
{
	friend struct AchivementEvent;

	AchivementMgr();
	~AchivementMgr();
	DECLARE_SINGLETON(AchivementMgr)

public:

	bool Init();
	bool Reload();
	void Uninit();

	void RegistHandler(IAchivementHandler *pHandler);
	int  DoCompleteAchivement(Role *pRole, int aid);

private:

	void OnAchivementEvent(AchivementEvent &evt);

	AchivementTable m_oTable;
	IAchivementHandler* m_Handlers[ACHEVT_MAX];
};

#endif // __ACHIVEMENTMGR_H__