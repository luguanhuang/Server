using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace XMainClient
{
    public enum StateDef
    {
        Disconnect,
        Connected,
        Challenge,
        WaitAccountData,
        SelectRole,
        CreateRole,
        RecieveRoleData,
        EnterSceneReq,
        EnterScene,
        LeaveScene,
    }

    public enum SyncTimeState
    {
        Invalid,
        Start,
        Waiting,
        End,
    }

    public class ClientState
    {
        public static int LoginRoleCount = 0;

        public string Account;
        public string Password;
        public string script;
        public uint ServerID;
        public StateDef state;
        public SyncTimeState syncTimeState;
        public uint CurrentScene;
        public int basicProfession;
        private Motion motion;
        private Bag bag;
        private DateTime syncTime;
        private ulong serverTime;
        private Connection connection;
        private TaskChain taskchain;

        public StateDef State
        {
            get { return state; }
            set
            {
                state = value;
                //Console.WriteLine("state = " + value.ToString());
            }
        }

        public ClientState(Connection conn)
        {
            connection = conn;
            State = StateDef.Disconnect;
            syncTimeState = SyncTimeState.Invalid;
            taskchain = new TaskChain(this);
            motion = new Motion(this);
            bag = new Bag(this);
        }

        public string Script
        {
            set { script = value; taskchain.LoadTaskScript(script); }
        }

        public void Update()
        {
            taskchain.Update();
        }

        public Task GetCurrentTask()
        {
            return taskchain.GetCurrentTask();
        }

        public int GetCurrentTaskID()
        {
            return taskchain.GetTaskStep();
        }

        public void SetTaskStep(int step)
        {
            taskchain.SetTaskStep(step);
        }

        public void SetServerTime(long time)
        {
            serverTime = (ulong)time;
            syncTime = DateTime.Now;
            syncTimeState = SyncTimeState.End;
        }

        public ulong GetServerTime()
        {
            return serverTime + (ulong)(DateTime.Now - syncTime).TotalMilliseconds;
        }

        public void LeaveScene()
        {
            RpcC2G_ReportBattle oPtc = new RpcC2G_ReportBattle();
            oPtc.oArg.battledata.hppercent = 1;
            oPtc.oArg.battledata.timespan = 10;
            connection.Send(oPtc);

            PtcC2G_LeaveSceneReq oLeavePtc = new PtcC2G_LeaveSceneReq();
            connection.Send(oLeavePtc);

            State = StateDef.LeaveScene;
        }

        public void EnterScene(uint SceneID)
        {
            PtcC2G_EnterSceneReq oPtc = new PtcC2G_EnterSceneReq();
            oPtc.Data.sceneID = SceneID;
            connection.Send(oPtc);

            State = StateDef.EnterSceneReq;
        }

        public void Send(Protocol proto)
        {
            connection.Send(proto);
        }

        public void Send(Rpc rpc)
        {
            connection.Send(rpc);
        }

        public void MoveTo(double x, double z)
        {
            motion.MoveTo(x, z);
        }

        public void Close()
        {
            connection.Close();
        }

        public void InitRoleData(KKSG.RoleAllInfo roleData)
        {
            State = StateDef.RecieveRoleData;
            if(roleData.Brief.position!=null)
                motion.SetPosition(roleData.Brief.position);
            if(roleData.Bag != null)
                bag.InitItems(roleData.Bag);
        }

        public void ResetTaskChain()
        {
            taskchain.ResetTask();
        }

        internal Bag GetBag()
        {
            return bag;
        }

        internal bool Reconnect(string ip, int port)
        {
            return connection.Reconnect(ip, port);
        }
    }
}
