#ifndef __ATTRIBUTEPK_H__
#define __ATTRIBUTEPK_H__

#include "table/PkProfessionTable.h"

class Unit;
class Role;

class AttributePkMgr
{
	AttributePkMgr();
	~AttributePkMgr();
	DECLARE_SINGLETON(AttributePkMgr);

public:
	bool Init();
	void Uninit();

	bool CheckFile();
	bool LoadFile();
	void ClearFile();

	bool IsUseSceneType(int scenetype);

	void SetPkAttribute(Unit* unit, int scenetype);
	void AdjustAttrs(Role *pRole,KKSG::SceneType nSceneType,UINT32 nPPT,UINT32 nAvg);

private:
	void InitUseSceneType();
	PkProfessionTable::RowData* GetPkProConf(int profession, int scenetype);

private:
	PkProfessionTable m_PkProTable;
	std::unordered_set<int> m_usescenetype;
};

#endif