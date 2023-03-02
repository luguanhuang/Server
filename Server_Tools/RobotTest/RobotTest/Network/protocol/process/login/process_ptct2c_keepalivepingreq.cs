using System;
using System.Collections.Generic;

using System.Text;

// generate by ProtoGen at date: 2014/8/14 15:26:20

namespace XMainClient
{
	class Process_PtcT2C_KeepAlivePingReq
	{
		public static void Process(PtcT2C_KeepAlivePingReq roPtc)
		{
            PtcC2T_KeepAlivePingAck ackPtc = new PtcC2T_KeepAlivePingAck();
            ConnectionMgr.CurrentConnection.Send(ackPtc);
		}

	}
}