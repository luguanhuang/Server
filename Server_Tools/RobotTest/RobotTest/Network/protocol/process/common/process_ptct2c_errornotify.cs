using System;
using System.Collections.Generic;

using System.Text;
using KKSG;
// generate by ProtoGen at date: 2014/9/11 20:22:55


namespace XMainClient
{
	class Process_PtcT2C_ErrorNotify
	{
		public static void Process(PtcT2C_ErrorNotify roPtc)
		{
            ErrorCode code = (ErrorCode)roPtc.Data.errorno;
            Console.WriteLine("[" + ConnectionMgr.CurrentConnection.state.Account + "] Error: " + code.ToString());
		}
	}
}