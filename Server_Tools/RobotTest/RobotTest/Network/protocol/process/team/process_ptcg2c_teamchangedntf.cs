using System;
using System.Collections.Generic;
using System.Text;

// generate by ProtoGen at date: 2015/7/15 17:50:10

namespace XMainClient
{
	class Process_PtcG2C_TeamChangedNtf
	{
		public static void Process(PtcG2C_TeamChangedNtf roPtc)
		{
            Connection conn = ConnectionMgr.CurrentConnection;
            if (!(conn.state.GetCurrentTask() is TeamBattleTask))
                return;
            TeamBattleTask task = ((TeamBattleTask)conn.state.GetCurrentTask());
            KKSG.TeamChanged data = roPtc.Data;
            if (data.teamBrief.teamMemberCount > 1 && task.GetState() == TeamState.START_MATCH)
            {
                //Console.WriteLine(conn.state.Account + " team match success: " + task.expID);
                task.SetState(TeamState.MATCH_SUCESS);
                if(data.addMember.Count > 0)
                    task.otherMemberID = roPtc.Data.addMember[0].memberID;
            }
            //Console.WriteLine(conn.state.Account + " team battle statechange: " + task.expID + " state: "+ task.GetState() + " member:" + data.teamBrief.teamMemberCount + ","+data.teamBrief.leaderName + " othermember:" + task.otherMemberID );
        }

	}
}