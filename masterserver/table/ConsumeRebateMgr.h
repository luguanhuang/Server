#ifndef __ConsumeRebateMgr_H__
#define __ConsumeRebateMgr_H__
#include "table/ConsumeRebate.h"

enum ConsumeRebateDateType
{
	ConsumeRebateDateType1 = 1, //开服前多少天;
	ConsumeRebateDateType2 = 2, //指定日期;
	ConsumeRebateDateTypeMax
};

class CConsumeRebateConfig
{
	CConsumeRebateConfig();
	~CConsumeRebateConfig();
	DECLARE_SINGLETON(CConsumeRebateConfig)

public:
	bool Init();
	void Uninit();

	bool CheckFile(bool isReload);
	bool LoadFile(bool isReload = true);
	void ClearFile();

	UINT32 GetTimeFromStr(UINT32 Days);
	bool CheckOpen(ConsumeRebate::RowData* pRow);
	bool CheckDaySame(ConsumeRebate::RowData* pRowA, ConsumeRebate::RowData* pRowB);
	ConsumeRebate::RowData* GetOpenConfig();
private:
	ConsumeRebate m_oTable;
};
#endif
