using System;
using System.Collections.Generic;
using System.IO;
using ProtoBuf;
using System.Linq;

namespace XMainClient
{
    enum LoginState
    {
        INIT,
        SEND_QUERY_GATE,
        RECV_QUERY_GATE,
        CONNECT_GATE,
        GATE_CHALLENGE,
    }

    class LoginTask : Task
    {
        string Account;
        LoginState inState;
        byte[] loginToken;
        bool isLoginFinish;
        string gateip;
        int gateport;
        int Profession;

        public LoginTask(string account, string profession)
        {
            isLoginFinish = false;
            Account = account;
            inState = LoginState.INIT;
            Profession = int.Parse(profession);

            if (Profession == 0)
            {
                Random r = new Random();
                Profession = r.Next(4) + 1;
            }
        }

        public override void OnUpdate()
        {
            if (inState == LoginState.INIT)
            {
                if (state.State == StateDef.Connected)
                {
                    inState = LoginState.SEND_QUERY_GATE;

                    RpcC2I_QueryGateIP rpc = new RpcC2I_QueryGateIP();
                    rpc.oArg.account = Account;
                    rpc.oArg.password = "52071";
                    rpc.oArg.openid = Account;
                    rpc.oArg.type = KKSG.LoginType.LOGIN_PASSWORD;
                    state.Send(rpc);
                }
            }

            if (inState == LoginState.CONNECT_GATE)
            {
                state.Reconnect(gateip, gateport);
                inState = LoginState.GATE_CHALLENGE;
            }
            
            if (state.State == StateDef.Challenge)
            {
                RpcC2T_ClientLoginRequest loginPtc = new RpcC2T_ClientLoginRequest();
                loginPtc.oArg.token = loginToken;
                loginPtc.oArg.gameserverid = state.ServerID;
                loginPtc.oArg.pc = "0.0.0";
                loginPtc.oArg.openid = state.Account;
                Console.WriteLine("[" + state.Account + "] connect to server id " + state.ServerID);
                state.Send(loginPtc);
                state.State = StateDef.WaitAccountData;
            }
        }

        public override bool IsFinish()
        {
            return isLoginFinish;
        }

        public void ParseRoleBriefInfo(List<KKSG.RoleBriefInfo> list, byte[] data)
        {
            if (data.Length == 0)
            {
                list.Add(null);
            }
            else
            {
                MemoryStream ms = new MemoryStream(data);
                KKSG.RoleBriefInfo brief = Serializer.Deserialize<KKSG.RoleBriefInfo>(ms);
                list.Add(brief);
            }
        }

        static Random random = new Random(Environment.TickCount);
        static string chars = "_abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

        private void PrintRoleList(List<KKSG.RoleBriefInfo> list, int selectID)
        {
            Console.WriteLine("RoleList:");
            for (int i = 0; i < list.Count; ++i)
            {
                if (selectID == i)
                {
                    Console.Write("[*]");
                }
                else
                {
                    Console.Write("[ ]");
                }

                if (list[i] == null)
                {
                    Console.WriteLine(" Empty");
                }
                else
                {
                    Console.WriteLine(list[i].name + " level:" + list[i].level);
                }
            }
        }

        private string MakeRandomName(int length)
        {
            return new string(
                    Enumerable.Repeat(chars, length)
                              .Select(s => s[random.Next(s.Length)])
                              .ToArray());
        }

        public void OnAccountData(KKSG.LoadAccountData Data)
        {
            List<KKSG.RoleBriefInfo> list = new List<KKSG.RoleBriefInfo>();
            ParseRoleBriefInfo(list, Data.role1);
            ParseRoleBriefInfo(list, Data.role2);
            ParseRoleBriefInfo(list, Data.role3);
            ParseRoleBriefInfo(list, Data.role4);

            //PrintRoleList(list, (int)Data.selectSlot);

            bool bCreateRole = false;
            int selectID = (int)Data.selectSlot;
            if (list[Profession - 1] == null)
            {
                bCreateRole = true;
            }
            if(bCreateRole)
            {
                //RpcC2G_CreateRole createRole = new RpcC2G_CreateRole();
                //createRole.oArg.type = KKSG.RoleType.Role_Warrior;
                RpcC2M_CreateRoleNew createRole = new RpcC2M_CreateRoleNew();
                createRole.oArg.type = (KKSG.RoleType)Profession;
                createRole.oArg.name = MakeRandomName(10);
                state.Send(createRole);
                state.State = StateDef.CreateRole;
                state.basicProfession = Profession;
            }
            if (!bCreateRole)
            {
                //RpcC2G_ClientSelectRole selectRole = new RpcC2G_ClientSelectRole();
                RpcC2M_SelectRoleNew selectRole = new RpcC2M_SelectRoleNew();
                selectRole.oArg.index = Profession - 1;
                state.Send(selectRole);
                state.State = StateDef.SelectRole;
                state.basicProfession = Profession;
            }
        }

        public void OnCreateRole(KKSG.ErrorCode code, KKSG.RoleAllInfo role)
        {
            if (code == KKSG.ErrorCode.ERR_SUCCESS)
            {
                Console.WriteLine("Create Role Ok");
                /*state.InitRoleData(role);
                isLoginFinish = true;

                ClientState.LoginRoleCount += 1;*/
                RpcC2M_SelectRoleNew selectRole = new RpcC2M_SelectRoleNew();
                selectRole.oArg.index = Profession - 1;
                state.Send(selectRole);
                state.State = StateDef.SelectRole;
                state.basicProfession = Profession;
            }
            else
            {
                Console.WriteLine("Create Role Failed");
            }
        }

        public void OnSelectRole(KKSG.ErrorCode code/*, KKSG.RoleAllInfo role*/)
        {
            if (code == KKSG.ErrorCode.ERR_SUCCESS)
            {
                Console.WriteLine("Select Role OK");
                //state.InitRoleData(role);
                //isLoginFinish = true;

                //ClientState.LoginRoleCount += 1;
            }
            else
            {
                Console.WriteLine("Select Role Failed!");
            }
        }

        public void OnSelectRoleReceiveData( KKSG.RoleAllInfo role)
        {
            Console.WriteLine("Select Role GetData OK");
            state.InitRoleData(role);
            isLoginFinish = true;

            ClientState.LoginRoleCount += 1;
        }

        public override void OnStart()
        {
        }
        public override void ResetTask()
        {
        }

        internal void OnQueryGateIP(KKSG.QueryGateRes oRes)
        {
            inState = LoginState.RECV_QUERY_GATE;
            if (oRes.loginToken.Length != 0)
            {
                Program.Conf.Instance.FillGate(oRes);
                loginToken = new byte[oRes.loginToken.Length];
                Array.Copy(oRes.loginToken, loginToken, oRes.loginToken.Length);

                inState = LoginState.CONNECT_GATE;

                if (oRes.RecommandGate.serverid == XMainClient.Program.Conf.Instance.serverID)
                {
                    Console.WriteLine("Query gate ok, connect gate: " + oRes.RecommandGate.ip + ":" + oRes.RecommandGate.port);
                    gateip = oRes.RecommandGate.ip;
                    gateport = (int)oRes.RecommandGate.port;
                }
                else
                {
                    var C = Program.Conf.Instance;
                    string gateaddr = C.GetGate(C.serverID);
                    if (gateaddr != null)
                    {
                        string[] addr = gateaddr.Split(':');
                        gateip = addr[0];
                        gateport = int.Parse(addr[1]);
                        Console.WriteLine("Use gate : " + gateip + ":" + gateport);
                    }
                    else
                    {
                        Console.WriteLine("Query gate failed!, not found serverid: {0}", C.serverID);
                    }
                }
            
            }
            else
            {
                state.Close();

                Console.WriteLine("Query gate failed, close connection");
            }
        }
    }
}
