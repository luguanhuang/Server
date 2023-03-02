using System;
using System.Collections.Generic;

using System.Text;

// generate by ProtoGen at date: 2014/8/13 15:53:48

namespace XMainClient
{
	class Process_PtcT2C_LoginChallenge
	{
		public static void Process(PtcT2C_LoginChallenge roPtc)
		{
            Console.WriteLine("[" + ConnectionMgr.CurrentConnection.state.Account + "] Recv Challenge");
            ConnectionMgr.CurrentConnection.state.State = StateDef.Challenge;
		}

	}
}