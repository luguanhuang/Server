using System;
using System.Collections.Generic;

using System.Text;
// generate by ProtoGen at date: 2015/1/23 11:18:07


namespace XMainClient
{
	class Process_PtcG2C_LevelChangeNtf
	{
		public static void Process(PtcG2C_LevelChangeNtf roPtc)
		{
            Console.WriteLine("[" + ConnectionMgr.CurrentConnection.state.Account + "] level to " + roPtc.Data.level);
		}
	}
}