#ifndef __ITEMBRIEF_H__
#define __ITEMBRIEF_H__

#include "define/itemdef.h"

namespace KKSG
{
	class Item;
	class UnitAppearance;
	class OutLookEquip;
}
struct XItem;
class FashionBrief
{
public:
	FashionBrief();
	~FashionBrief();

	UINT32 Size() const { return FashionCount; }
	UINT32 GetFashionPos(int i) const;
	void   SetFashionPos(int i, UINT32 itemID);
	void   SaveToAppearance(KKSG::UnitAppearance* pUnitAppearnce) const;
	UINT32 GetRandomFashion()const;
private:
	UINT32 m_dwFashion[FashionCount];
};

class EquipBrief
{
public:
	EquipBrief();
	~EquipBrief();

	EquipBrief(const EquipBrief &o);
	void operator=(const EquipBrief &o);

	const XItem* GetEquip(UINT32 pos);
	void SetEquip(UINT32 pos, const XItem& item);
	void SetEquip(UINT32 pos, const KKSG::Item& item);
	void ClearEquip(UINT32 pos);

private:
	void Clone(const EquipBrief &o);

	XItem* m_Equip[EquipCount];
};

class EmblemBrief
{
public:
	EmblemBrief();
	~EmblemBrief();

	EmblemBrief(const EmblemBrief &o);
	void operator=(const EmblemBrief &o);

	const XItem* GetEmblem(UINT32 pos);
	void SetEmblem(UINT32 pos, const XItem& item);
	void SetEmblem(UINT32 pos, const KKSG::Item& item);
	void ClearEmblem(UINT32 pos);

private:
	void Clone(const EmblemBrief &o);

	XItem* m_Emblem[EmblemCount];
};

class ArtifactBrief
{
public:
	ArtifactBrief();
	~ArtifactBrief();

	ArtifactBrief(const ArtifactBrief &o);
	void operator=(const ArtifactBrief &o);

	const XItem* GetArtifact(UINT32 pos);
	void SetArtifact(UINT32 pos, const XItem& item);
	void SetArtifact(UINT32 pos, const KKSG::Item& item);
	void ClearArtifact(UINT32 pos);
	void SaveToAppearance(KKSG::UnitAppearance* pUnitAppearnce);

private:
	void Clone(const ArtifactBrief &o);

	XItem* m_Artifact[ArtifactCount];
};


#endif
