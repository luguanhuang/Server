#include "pch.h"
#include "payibitemtask.h"

CPayIbItemTask::CPayIbItemTask()
	:CMysqlTask("PayIbItemTask")
{
	m_Op = 0;
}

CPayIbItemTask::~CPayIbItemTask()
{

}

void CPayIbItemTask::SetData(KKSG::MysqlOpType op, const KKSG::PayGiftIbItem& oData)
{
	m_Op = op;
	m_oData = oData;
}

void CPayIbItemTask::Execute(IMysqlConn* poMysqlConn, char* pcBuffer, INT32 nLength)
{
	SetMysqlError(MYSQL_ERR_FAILED);
	std::stringstream ss;
	if (-1 == poMysqlConn->EscapeString(m_oData.text().c_str(), (INT32)m_oData.text().size(), pcBuffer, nLength)) return;
	if (m_Op == KKSG::MYSQL_OP_ADD)
	{
		ss << "insert into payibitem(`billno`,`fromroleid`,`toroleid`,`goodsid`,`itemid`,`count`,`paydegree`,`text`,`ts`,`status`) values('" << m_oData.billno() << "'," << m_oData.fromroleid() << "," << m_oData.toroleid() << ",'" << m_oData.goodsid() << "'," << m_oData.itemid() << "," << m_oData.count() <<"," << m_oData.paydegree()<< ",'" << pcBuffer << "'," << m_oData.ts() << "," << m_oData.status() << ")";
	}
	else if (m_Op == KKSG::MYSQL_OP_UPDATE)
	{
		ss << "update payibitem set `fromroleid`=" << m_oData.fromroleid() << ",`toroleid`=" << m_oData.toroleid() << ",`goodsid`='" << m_oData.goodsid() <<  "',`itemid`=" << m_oData.itemid() <<  ",`count`=" << m_oData.count() << ",text='" << pcBuffer << "',`ts`=" << m_oData.ts() << ",`status`=" << m_oData.status() << " where `billno`='" << m_oData.billno() << "'"; 
	}
	else if (m_Op == KKSG::MYSQL_OP_DELETE)
	{
		ss << "delete from payibitem where `billno`='" << m_oData.billno() << "'";
	}
	else
	{
		GetErrStream() << "error op type=" << m_Op;
		return;
	}
	if(!poMysqlConn->Query(ss.str().c_str(), ss.str().size(), NULL))
	{
		GetErrStream() << "ErrDesc: " << poMysqlConn->GetError() << ", ErrNo: " << poMysqlConn->GetErrno();
		return;
	}

	SetMysqlError(MYSQL_ERR_SUCESS);
}

void CPayIbItemTask::OnReply()
{
	if(m_nMysqlErr != MYSQL_ERR_SUCESS)
	{
		LogError("Write payibitem data task failed, roleid: %llu, errMsg: %s", m_oData.fromroleid(), GetErrStream().str().c_str());
	}
}

