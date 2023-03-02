#include "pch.h"
#include "time.h"
#include "pkopenmgr.h"
#include "globalconfig.h"
#include "util/XCommon.h"

//INSTANCE_SINGLETON(PkOpenMgr);
//
//bool PkOpenMgr::Init()
//{
//	if (!m_table.LoadFile("table/PkOpen.txt"))
//	{
//		SSWarn<<"load file table/PkOpen.txt failed"<<END;
//		return false;
//	}
//
//	time_t nowtime = time(NULL);
//	tm* nowtimenode = localtime(&nowtime);
//	for (auto i = m_table.Table.begin(); i != m_table.Table.end(); ++i)
//	{
//		PkOpenTable::RowData* rowdata = *i;
//
//		PkOpenNode node;
//		node.begin.hour = rowdata->timebegin[0];
//		node.begin.min = rowdata->timebegin[1];
//		node.begin.sec = rowdata->timebegin[2];
//		node.end.hour = rowdata->timeend[0];
//		node.end.min = rowdata->timeend[1];
//		node.end.sec = rowdata->timeend[2];
//		
//		if (rowdata->id == 1)
//		{
//			node.id = 1;
//			m_workday = node;
//		}
//		else if (rowdata->id == 2)
//		{
//			node.id = 2;
//			m_weekend = node;
//		}
//		else
//		{
//			node.id = rowdata->id;
//			nowtimenode->tm_year = rowdata->date[0] - 1900;
//			nowtimenode->tm_mon = rowdata->date[1] - 1;
//			nowtimenode->tm_mday = rowdata->date[2];
//			nowtimenode->tm_hour = 0;
//			nowtimenode->tm_min = 0;
//			nowtimenode->tm_sec = 0;
//
//			m_opens[mktime(nowtimenode)] = node;
//		}
//	}
//
//	return true;
//}
//
//void PkOpenMgr::Uninit()
//{
//
//}
//
//UINT32 PkOpenMgr::TimeLimit()
//{
//	PkOpenNode node;
//	time_t nowtime = time(NULL);
//	tm* timenode = localtime(&nowtime);
//
//	time_t daybegin = XCommon::GetDayBeginTime(nowtime);
//	auto i = m_opens.find(daybegin);
//	if (i != m_opens.end())
//	{
//		node = i->second;
//	}
//	else
//	{
//		if (0 == timenode->tm_wday || 6 == timenode->tm_wday)
//		{
//			node = m_weekend;
//		}
//		else
//		{
//			node = m_workday;
//		}
//	}
//	if (XCommon::IsWhithinTime(node.begin.hour, node.begin.min, node.begin.sec, node.end.hour, node.end.min, node.end.sec))
//	{
//		return 0;
//	}
//	else
//	{
//		return node.id;
//	}
//}