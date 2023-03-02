using System;
using System.Collections.Generic;
using System.Text;

// generate by ProtoGen at date: 2015/8/10 13:47:08

namespace XMainClient
{
	class Process_PtcG2C_EnterSceneFinally
	{
		public static void Process(PtcG2C_EnterSceneFinally roPtc)
		{
            //Console.WriteLine("Enter Scene Finally");

            ConnectionMgr.CurrentConnection.state.State = StateDef.EnterScene;
		}
	}
}