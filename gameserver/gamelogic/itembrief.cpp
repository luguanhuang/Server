#include "pch.h"
#include "itembrief.h"
#include "item.h"
#include "pb/project.pb.h"
#include "util/XRandom.h"
#include "globalconfig.h"
#include "emblemmgr.h"


UINT32 FashionBrief::GetFashionPos(int i) const
{
	if (i >= 0 && i < FashionCount)
	{
		return m_dwFashion[i];
	}

	return 0;
}

void FashionBrief::SetFashionPos(int i, UINT32 itemID)
{
	if (i >= 0 && i < FashionCount)
	{
		m_dwFashion[i] = itemID;
	}
}

FashionBrief::FashionBrief()
{
	memset(m_dwFashion, 0, sizeof(m_dwFashion));
}

FashionBrief::~FashionBrief()
{
}

void FashionBrief::SaveToAppearance(KKSG::UnitAppearance* pUnitAppearnce) const
{
	for (int i = 0; i < FashionCount; ++i)
	{
		pUnitAppearnce->add_fashion(m_dwFashion[i]);
	}
}

UINT32 FashionBrief::GetRandomFashion() const
{
	int k = XRandom::randInt(0, (UINT32)FashionCount);
	for(int i = 0; i < FashionCount; ++i)
	{
		int index = (k + i)%(FashionCount);
		if(m_dwFashion[index])
		{
			return m_dwFashion[index];
		}
	}
	return 0;
}

EquipBrief::EquipBrief()
{
	memset(m_Equip, 0, sizeof(m_Equip));
}

void EquipBrief::Clone(const EquipBrief &o)
{
	for (int i = 0; i < EquipCount; ++i)
	{
		if (NULL != o.m_Equip[i])
		{
			m_Equip[i] = new XItem;
			*m_Equip[i] = *o.m_Equip[i];
		}
		else
		{
			m_Equip[i] = NULL;
		}
	}
}

void EquipBrief::operator=(const EquipBrief &o)
{
	Clone(o);
}

EquipBrief::EquipBrief(const EquipBrief &o)
{
	Clone(o);
}

EquipBrief::~EquipBrief()
{
	for (int i = 0; i < EquipCount; ++i)
	{
		if (NULL != m_Equip[i])
		{
			delete m_Equip[i];
			m_Equip[i] = NULL;
		}
	}
}

const XItem* EquipBrief::GetEquip(UINT32 pos)
{
	if (pos >= EquipCount)
	{
		return NULL;
	}
	return m_Equip[pos];
}

void EquipBrief::SetEquip(UINT32 pos, const XItem& item)
{
	if (pos >= EquipCount)
	{
		return;
	}
	if (NULL == m_Equip[pos])
	{
		m_Equip[pos] = new XItem;
	}
	*m_Equip[pos] = item;
}

void EquipBrief::SetEquip(UINT32 pos, const KKSG::Item& item)
{
	if (pos >= EquipCount)
	{
		return;
	}
	if (NULL == m_Equip[pos])
	{
		m_Equip[pos] = new XItem;
	}
	XItem::KKSGItem2Item(m_Equip[pos], &item);
}
	
void EquipBrief::ClearEquip(UINT32 pos)
{
	if (pos >= EquipCount)
	{
		return;
	}
	if (NULL != m_Equip[pos])
	{
		delete m_Equip[pos];
		m_Equip[pos] = NULL;
	}
}

EmblemBrief::EmblemBrief()
{
	memset(m_Emblem, 0, sizeof(m_Emblem));
}

EmblemBrief::~EmblemBrief()
{
	for (int i = 0; i < EmblemCount; ++i)
	{
		if (NULL != m_Emblem[i])
		{
			delete m_Emblem[i];
			m_Emblem[i] = NULL;
		}
	}
}

EmblemBrief::EmblemBrief(const EmblemBrief &o)
{
	Clone(o);
}

void EmblemBrief::operator=(const EmblemBrief &o)
{
	Clone(o);
}

const XItem* EmblemBrief::GetEmblem(UINT32 pos)
{
	if (pos >= EmblemCount)
	{
		return NULL;
	}
	return m_Emblem[pos];
}

void EmblemBrief::SetEmblem(UINT32 pos, const XItem& item)
{
	if (pos >= EmblemCount)
	{
		return;
	}
	if (NULL == m_Emblem[pos])
	{
		m_Emblem[pos] = new XItem;
	}
	*m_Emblem[pos] = item;
}

void EmblemBrief::SetEmblem(UINT32 pos, const KKSG::Item& item)
{
	if (pos >= EmblemCount)
	{
		return;
	}
	if (NULL == m_Emblem[pos])
	{
		m_Emblem[pos] = new XItem;
	}
	XItem::KKSGItem2Item(m_Emblem[pos], &item);
}

void EmblemBrief::ClearEmblem(UINT32 pos)
{
	if (pos >= EmblemCount)
	{
		return;
	}
	if (NULL != m_Emblem[pos])
	{
		delete m_Emblem[pos];
		m_Emblem[pos] = NULL;
	}
}

void EmblemBrief::Clone(const EmblemBrief &o)
{
	for (int i = 0; i < EmblemCount; ++i)
	{
		if (NULL != o.m_Emblem[i])
		{
			m_Emblem[i] = new XItem;
			*m_Emblem[i] = *o.m_Emblem[i];
		}
		else
		{
			m_Emblem[i] = NULL;
		}
	}
}

ArtifactBrief::ArtifactBrief()
{
	memset(m_Artifact, 0, sizeof(m_Artifact));
}

ArtifactBrief::~ArtifactBrief()
{
	for (int i = 0; i < ArtifactCount; ++i)
	{
		if (NULL != m_Artifact[i])
		{
			delete m_Artifact[i];
			m_Artifact[i] = NULL;
		}
	}
}

ArtifactBrief::ArtifactBrief(const ArtifactBrief &o)
{
	Clone(o);
}

void ArtifactBrief::operator=(const ArtifactBrief &o)
{
	Clone(o);
}

const XItem* ArtifactBrief::GetArtifact(UINT32 pos)
{
	if (pos >= ArtifactCount)
	{
		return NULL;
	}
	return m_Artifact[pos];
}

void ArtifactBrief::SetArtifact(UINT32 pos, const XItem& item)
{
	if (pos >= ArtifactCount)
	{
		return;
	}
	if (NULL == m_Artifact[pos])
	{
		m_Artifact[pos] = new XItem;
	}
	*m_Artifact[pos] = item;
}

void ArtifactBrief::SetArtifact(UINT32 pos, const KKSG::Item& item)
{
	if (pos >= ArtifactCount)
	{
		return;
	}
	if (NULL == m_Artifact[pos])
	{
		m_Artifact[pos] = new XItem;
	}
	XItem::KKSGItem2Item(m_Artifact[pos], &item);
}

void ArtifactBrief::ClearArtifact(UINT32 pos)
{
	if (pos >= ArtifactCount)
	{
		return;
	}
	if (NULL != m_Artifact[pos])
	{
		delete m_Artifact[pos];
		m_Artifact[pos] = NULL;
	}
}

void ArtifactBrief::Clone(const ArtifactBrief &o)
{
	for (int i = 0; i < ArtifactCount; ++i)
	{
		if (NULL != o.m_Artifact[i])
		{
			m_Artifact[i] = new XItem;
			*m_Artifact[i] = *o.m_Artifact[i];
		}
		else
		{
			m_Artifact[i] = NULL;
		}
	}
}
