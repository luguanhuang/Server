#ifndef __ARTIFACTCONFIG_H__
#define __ARTIFACTCONFIG_H__

#include "util/utilsingleton.h"
#include "table/ArtifactListTable.h"
#include "table/ArtifactSuitTable.h"
#include "table/ArtifactComposeTable.h"
#include "table/ArtifactSkillInscription.h"


typedef ArtifactListTable::RowData ArtifactListConf;
typedef ArtifactSuitTable::RowData ArtifactSuitConf;
typedef ArtifactComposeTable::RowData ArtifactComposeConf;
typedef ArtifactSkillInscription::RowData ArtifactInscriptionConf;


class ArtifactTableData
{
public:
	ArtifactTableData();
	~ArtifactTableData();

	bool HandlerTableData();

protected:
	ArtifactListTable m_oListTable;
	ArtifactSuitTable m_oSuitTable;
	ArtifactComposeTable m_oComposeTable;
	ArtifactSkillInscription m_oInscriptTable;

	std::map<std::pair<UINT32, UINT32>, ArtifactComposeConf*> m_composeMap;
};

struct XItem;

class ArtifactConfig : public Singleton<ArtifactConfig>, public ArtifactTableData
{
public:
	ArtifactConfig();
	~ArtifactConfig();

	bool Init();
	void Uninit();

	bool CheckFile();
	bool LoadFile();
	void ClearFile();

	const ArtifactListConf* GetArtifactListConf(UINT32 artifactID) { return m_oListTable.GetByArtifactID(artifactID); }

	const ArtifactSuitConf* GetArtifactSuitConf(UINT32 suitID) { return m_oSuitTable.GetByArtifactSuitID(suitID); }
	
	const ArtifactInscriptionConf* GetArtifactInscriptionConf(UINT32 itemID) { return m_oInscriptTable.GetByItemID(itemID); }

	void InitArtifactAttribute(XItem* pItem, bool isRecast);

	UINT32 GetEffectIDDrop(UINT32 effectDrop);

	const ArtifactComposeConf* GetArtifactComposeConf(UINT32 level, UINT32 quality);

	void GetArtifactAttrRange(UINT32 artifactID, UINT32 index, UINT32 attrid, UINT32& attrfirst, UINT32& attrsecond);

private:
	void _InitOneAttribute(XItem* pItem, const std::vector<Sequence<UINT32, 4>>& attrs);
	void _RecastOneAttribute(XItem* pItem, const std::vector<Sequence<UINT32, 4>>& attrs);
	
};


#endif