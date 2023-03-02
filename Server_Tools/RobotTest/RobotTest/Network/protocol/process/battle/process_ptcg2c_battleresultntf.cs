using System;

// generate by ProtoGen at date: 2015/2/2 19:20:03

namespace XMainClient
{
	class Process_PtcG2C_BattleResultNtf
	{
		public static void Process(PtcG2C_BattleResultNtf roPtc)
		{
            Connection conn = ConnectionMgr.CurrentConnection;

            if (conn.state.GetCurrentTask() is TeamBattleTask)
            {
                TeamBattleTask task = ((TeamBattleTask)conn.state.GetCurrentTask());
                if (conn.state.GetCurrentTask().GetTaskType() == TaskType.TYPE_TEAMTASK)
                {
                    task.Finish();
                    //task.SetState(TeamState.TEAM_BATTLE_END_CHANGESCENE);
                    Console.WriteLine(conn.state.Account + " team battle success expID: " + task.expID + " state: "+ task.GetState());
                }
            }
            
		}
	}
}