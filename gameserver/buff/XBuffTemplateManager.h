#ifndef __XBUFFTEMPLATEMANAGER_H__
#define __XBUFFTEMPLATEMANAGER_H__

#include "table/BuffTable.h"

class XBuff;
class CombatEffectHelper;

struct BuffDesc
{
	int buffID;
	int BuffLevel;
	UINT64 casterID;
	float delayTime;
	UINT32 effecttime;		// 单位： 秒
	UINT32 skillID;
	UINT32 type;	///> Buff来源类型，0：普通；1：Doodad

	bool IsTimeSpecified() { return effecttime != (UINT32)(-1); }

	BuffDesc(int _id, int _level, UINT32 _skillID = 0)
	{
		buffID = _id;
		BuffLevel = _level;
		casterID = 0;
		delayTime = 0;
		effecttime = (UINT32)(-1);
		skillID = _skillID;
		type = 0;
	}

	BuffDesc()
	{
		Reset();
	}

	void Reset()
	{
		buffID = 0;
		BuffLevel = 0;
		casterID = 0;
		delayTime = 0;
		effecttime = (UINT32)(-1);
		skillID = 0;
		type = 0;
	}

};
class Scene;
class XBuffTemplateManager
{
	XBuffTemplateManager();
	~XBuffTemplateManager();
	DECLARE_SINGLETON(XBuffTemplateManager)

public:

	bool Init();
	void Uninit();

	bool CheckFile();
	bool LoadFile();
	void ClearFile();
	
	XBuff *CreateBuff(const BuffDesc &buffDesc, CombatEffectHelper* pEffectHelper);
	BuffTable::RowData* GetBuffInfo(int buffID, int BuffLevel, Scene* pScene);

	bool IsScaleBuff(int buffid);

private:
	BuffTable m_BuffTable;
	BuffTable m_BuffTablePVP;
	std::map<int, std::set<UINT32>> m_buff2scenesInc;
	std::map<int, std::set<UINT32>> m_buff2scenesExc;
	std::unordered_map<int, BuffTable::RowData*> m_BuffIndex;
	std::unordered_map<int, BuffTable::RowData*> m_BuffIndexPVP;
};

#endif // __XBUFFTEMPLATEMANAGER_H__