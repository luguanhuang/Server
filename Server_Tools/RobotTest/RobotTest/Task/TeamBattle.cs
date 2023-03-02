using System;
using System.Collections.Generic;
using System.IO;
using ProtoBuf;
using System.Linq;

namespace XMainClient
{
    enum TeamState
    {
        NULL_STATE,
        REQ_CREATETEAM,
        CREATE_TEAM,
        START_MATCH,
        MATCH_SUCESS,
        TEAM_WAIT_START,
        TEAM_IN_BATTLE,
        TEAM_BATTLE_END_CHANGESCENE,
        TEAM_BATTLE_END,
        TEAM_BATTLE_TOTAL_END,
    }

    class TeamBattleTask : Task
    {
        public static Dictionary<TeamState, int> Total = new Dictionary<TeamState, int>();

        public int expID = 0;
        public bool isFinish = false;
        public UInt64 otherMemberID = 0;

        TeamState teamState;

        static TeamBattleTask()
        {
            Total.Add(TeamState.NULL_STATE, 0);
            Total.Add(TeamState.REQ_CREATETEAM, 0);
            Total.Add(TeamState.CREATE_TEAM, 0);
            Total.Add(TeamState.START_MATCH, 0);
            Total.Add(TeamState.MATCH_SUCESS, 0);
            Total.Add(TeamState.TEAM_WAIT_START, 0);
            Total.Add(TeamState.TEAM_IN_BATTLE, 0);
            Total.Add(TeamState.TEAM_BATTLE_END_CHANGESCENE, 0);
            Total.Add(TeamState.TEAM_BATTLE_END, 0);
            Total.Add(TeamState.TEAM_BATTLE_TOTAL_END, 0);
        }

        public TeamBattleTask(string expid)
        {
            expID = int.Parse(expid);
            taskType = TaskType.TYPE_TEAMTASK;
            teamState = TeamState.NULL_STATE;
            Total[teamState] += 1;
        }

        public override void OnUpdate()
        {
            RpcC2M_TeamRequestC2M teamRpc = new RpcC2M_TeamRequestC2M();
            if (teamState == TeamState.NULL_STATE)
            {
                teamRpc.oArg.request = KKSG.TeamOperate.TEAM_CREATE;
                teamRpc.oArg.expID = (uint)(expID);
                SetState(TeamState.REQ_CREATETEAM);
            }
            else if (teamState == TeamState.CREATE_TEAM)
            {
                /*teamRpc.oArg.request = KKSG.TeamOperate.TEAM_START_MATCH;
                teamRpc.oArg.expID = (uint)(expID);*/
                SetState(TeamState.START_MATCH);
                return;
            }
            else if (teamState == TeamState.MATCH_SUCESS)
            {
                //   RpcC2G_TeamRequest teamRpc = new RpcC2G_TeamRequest();
                teamRpc.oArg.request = KKSG.TeamOperate.TEAM_START_BATTLE;
                teamRpc.oArg.expID = (uint)(expID);
                SetState(TeamState.TEAM_WAIT_START);
            }
            else
                return;

            //Console.WriteLine(teamRpc.oArg.request + "  " + teamRpc.oArg.expID);
            //Console.WriteLine("send "+state.Account + " team battle expID: " + expID + " state: "+ GetState());
            state.Send(teamRpc);
        }

        public override bool IsFinish()
        {
            return teamState == TeamState.TEAM_BATTLE_END;
        }
        public override void ResetTask()
        {
            //teamState = TeamState.NULL_STATE;
            if (TeamState.TEAM_BATTLE_END == teamState)
            {
                SetState(TeamState.NULL_STATE);
            }
        }


        public override void OnStart()
        {
            //Console.WriteLine("[" + state.Account + "] teambattle : " + expID);
            if (TeamState.TEAM_BATTLE_END == teamState)
            {
                SetState(TeamState.NULL_STATE);
            }
        }

        public void SetState(TeamState teamstate_)
        {
            Total[teamState] -= 1;
            Total[teamstate_] += 1;

            this.teamState = teamstate_;
        }

        public TeamState GetState()
        {
            return teamState;
        }

        public void Finish()
        {
            //state.LeaveScene();
            SetState(TeamState.TEAM_BATTLE_END);
            Total[TeamState.TEAM_BATTLE_TOTAL_END] += 1;
        }
    }
}
