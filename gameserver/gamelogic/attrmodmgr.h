#ifndef __ATTRMODMGR_H__
#define __ATTRMODMGR_H__

#include "table/AttrModifyTable.h"
#include "table/PVEAttrModify.h"

class Role;
class Scene;
class AttrModMgr
{
	DECLARE_SINGLETON(AttrModMgr);
public:
	bool Init();
	void Uninit();

	void ClearFile();
	bool CheckFile();
	bool LoadFile();

	void ModifyAttr(Role* role, Scene* scene);

private:
	void Modify(Role* role, Scene* scene);
	void GetSceneInfo(bool& isfair, UINT32& scenetype, Scene* scene);

	double GetPer(double value, std::vector<Sequence<float, 3>>& valuetemplate);
	AttrModifyTable::RowData* GetData(Scene* scene);

private:
	AttrModifyTable m_table;
	std::map<UINT32, AttrModifyTable::RowData*> m_sceneid2data;
	std::map<UINT32, AttrModifyTable::RowData*> m_scenetype2data;

    PVEAttrModify m_PveAttrModifyTable;
    std::map<UINT32, PVEAttrModify::RowData*> m_PveAttrModify;
    void PveModify(Role* pRole, Scene* pScene);
    double pvemax(double a, double b);
};

#endif