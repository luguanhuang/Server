#include "table/Prerogative.h"
#include "table/PrerogativeContent.h"
#include "pb/project.pb.h"

struct EqualPreID
{
	UINT32 ID;
	EqualPreID(UINT32 iID) { ID = iID;}
	bool operator()(UINT32 id)
	{
		return id == ID;
	}
};
class CPreprogativeConfig
{
	CPreprogativeConfig();
	~CPreprogativeConfig();
	DECLARE_SINGLETON(CPreprogativeConfig);

public:
	bool Init();
	void Uninit();

	bool LoadFile();
	bool CheckFile();
	void ClearFile();
	PrerogativeContent::RowData* GetPrerogativeConf(UINT32 ID){	return m_oTable.GetByID(ID); }

private:
	PrerogativeContent m_oTable;
};
