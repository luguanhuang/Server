using System;
using System.Collections.Generic;

using System.Text;


// generate by ProtoGen at date: 2015/2/4 19:48:20

namespace XMainClient
{
	class Process_PtcG2C_CompleteAchivement
	{
		public static void Process(PtcG2C_CompleteAchivement roPtc)
		{
            if (roPtc.Data.state == 1)
            {
                RpcC2G_FetchAchivementReward fetchAchivement = new RpcC2G_FetchAchivementReward();
                fetchAchivement.oArg.AchivementID = roPtc.Data.achivementID;
                ConnectionMgr.CurrentConnection.Send(fetchAchivement);
            }
            else if (roPtc.Data.state == 2)
            {
                Console.WriteLine("[" + ConnectionMgr.CurrentConnection.state.Account + "] fetch achievement: " + roPtc.Data.achivementID);
            }
        }
	}
}