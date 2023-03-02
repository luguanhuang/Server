#include "pch.h"
//#include <assert.h>
//#include "fashionchallengemgr.h"
//#include "utility/XRandom.h"
//
//INSTANCE_SINGLETON(FashionChallengeMgr)
//
//FashionChallengeMgr::FashionChallengeMgr()
//{
//
//}
//
//FashionChallengeMgr::~FashionChallengeMgr()
//{
//
//}
//
//bool FashionChallengeMgr::Init()
//{
//	if (!m_oTable.LoadFile("table/FashionChallenge.txt"))
//	{
//		LogWarn("Load file table/FashionChallenge.txt failed!");
//		return false;
//	}
//
//	m_TotalProb = 0;
//	for (unsigned int i = 0; i < m_oTable.Table.size(); ++i)
//	{
//		FashionChallenge::RowData *Row = m_oTable.Table[i];
//		//assert(i+1 == Row->ID);
//		m_TotalProb += Row->Chance;
//	}
//	return true;
//}
//
//void FashionChallengeMgr::Uninit()
//{
//
//}
//
//FashionChallenge::RowData * FashionChallengeMgr::GetFashionChallengeConf(int id)
//{
//	assert(id > 0 && id <= (int)m_oTable.Table.size());
//	return m_oTable.Table[id-1];
//}
//
//FashionChallenge::RowData * FashionChallengeMgr::GetRandomChallenge()
//{
//	float prob = (float)XRandom::randDouble(0, m_TotalProb);
//	for (unsigned int i = 0; i < m_oTable.Table.size(); ++i)
//	{
//		FashionChallenge::RowData *Row = m_oTable.Table[i];
//		if (prob < Row->Chance)
//		{
//			return Row;
//		}
//		else
//		{
//			prob -= Row->Chance;
//		}
//	}
//
//	return m_oTable.Table.back();
//}
//
//int FashionChallengeMgr::GetRandomChallengeID()
//{
//	return GetRandomChallenge()->ID;
//}
//
