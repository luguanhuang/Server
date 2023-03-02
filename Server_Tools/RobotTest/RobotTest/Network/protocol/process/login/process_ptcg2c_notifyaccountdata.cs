using System;
using System.Collections.Generic;

using System.Text;
// generate by ProtoGen at date: 2014/8/13 15:58:42

using ProtoBuf;
using System.IO;

namespace XMainClient
{
	class Process_PtcG2C_NotifyAccountData
	{
		public static void Process(PtcG2C_NotifyAccountData roPtc)
		{
            LoginTask task = ConnectionMgr.CurrentConnection.GetTask() as LoginTask;
            if (task != null)
            {
                task.OnAccountData(roPtc.Data);
            }
		}
	}
}