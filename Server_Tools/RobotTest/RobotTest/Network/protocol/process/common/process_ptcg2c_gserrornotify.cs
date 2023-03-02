using System;
using System.Collections.Generic;
using System.Text;
using KKSG;

// generate by ProtoGen at date: 2015/6/10 16:49:43

namespace XMainClient
{
	class Process_PtcG2C_GSErrorNotify
	{
		public static void Process(PtcG2C_GSErrorNotify roPtc)
		{
            ErrorCode code = (ErrorCode)roPtc.Data.errorno;
            Console.WriteLine("[" + ConnectionMgr.CurrentConnection.state.Account + "] Error: " + code.ToString());
		}
	}
}