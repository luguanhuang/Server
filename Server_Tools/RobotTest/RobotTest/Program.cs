using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Collections;
using System.Net.Sockets;
using System.Net;
using System.Threading;
using System.IO;
using XUtliPoolLib;
using UnityEngine;

namespace XMainClient
{
    class Program
    {
        static ConnectionMgr mgr;
        static GameProtocolHandler handler;

        class OptGet
        {
            public delegate void OptHandler1();
            public delegate bool OptHandler2(string optarg);

            private Dictionary<string, OptHandler1> handlers1;
            private Dictionary<string, OptHandler2> handlers2;
            private OptHandler2 defaultHandler;

            public OptGet()
            {
                handlers1 = new Dictionary<string, OptHandler1>();
                handlers2 = new Dictionary<string, OptHandler2>();
            }

            public void SetDefaultHandler(OptHandler2 handler)
            {
                defaultHandler = handler;
            }

            public void AddOptHandler(string opt, OptHandler1 handler)
            {
                handlers1["-" + opt] = handler;
            }

            public void AddOptHandler(string opt, OptHandler2 handler)
            {
                handlers2["-" + opt] = handler;
            }

            public bool Parse(string[] args)
            {
                int i;
                for (i = 0; i < args.Length; )
                {
                    if (handlers1.ContainsKey(args[i]))
                    {
                        OptHandler1 handler;
                        handlers1.TryGetValue(args[i], out handler);
                        handler();
                        ++i;
                        continue;
                    }

                    if (handlers2.ContainsKey(args[i]))
                    {
                        OptHandler2 handler;
                        handlers2.TryGetValue(args[i], out handler);
                        if (i == args.Length - 1)
                        {
                            if (!handler(""))
                            {
                                return false;
                            }
                        }
                        else
                        {
                            if (!handler(args[i+1]))
                            {
                                return false;
                            }
                        }
                        
                        i += 2;
                        continue;
                    }

                    if (defaultHandler != null)
                    {
                        defaultHandler(args[i]);
                    }
                    else
                    {
                        return false;
                    }
                    
                    ++i;
                }

                return true;
            }

            public string Help()
            {
                StringBuilder builder = new StringBuilder();
                foreach (var kv in handlers1)
                {
                    builder.AppendFormat(" {0} ", kv.Key);
                }

                foreach (var kv in handlers2)
                {
                    builder.AppendFormat(" {0} {1} ", kv.Key, kv.Key.Substring(1));
                }

                return builder.ToString();
            }
        }

        public class Conf
        {
            public struct GateServerInfo
            {
                public uint serverID;
                public string IPPort;
            }

            public static Conf Instance = null;

            public string accountPrefix;
            public string account;
            public string ip;
            public string script;
            //public string url;
            public int port;
            public uint serverID;
            public int accountStart;
            public int accountEnd;
            public bool single;
            public List<GateServerInfo> gateips;

            public Conf()
            {
                accountPrefix = "A";
                ip = "10.0.128.229";
                port = 25001;
                account = "45";
                serverID = 1;
                script = "test.txt";
                single = false;
                gateips = new List<GateServerInfo>();

                if (Instance == null)
                {
                    Instance = this;
                }
            }

            public bool HandleIP(string ip_)
            {
                ip = ip_;
                return true;
            }

            public bool HandleAccount(string account_)
            {
                account = account_;
                return true;
            }

            public bool HandlePort(string port_)
            {
                return int.TryParse(port_, out port);
            }

            public bool HandleServerID(string serverid_)
            {
                return uint.TryParse(serverid_, out serverID);
            }

            public bool HandleScript(string script_)
            {
                script = script_;
                return true;
            }

            public bool HandleAccountStart(string arg)
            {
                return int.TryParse(arg, out accountStart);
            }

            public bool HandleAccountEnd(string arg)
            {
                return int.TryParse(arg, out accountEnd);
            }

            public bool HandleAccountPrefix(string arg)
            {
                accountPrefix = arg;
                return true;
            }

            public void HandleSingle()
            {
                single = true;
            }

            public bool HandlerURL(string arg)
            {
                Console.WriteLine("load gateip from: {0}", arg);
                
                HttpWebRequest httpReq; 
                HttpWebResponse httpResp; 

                Uri httpURL = new Uri(arg);
                httpReq = (HttpWebRequest)WebRequest.Create(httpURL); 
                httpResp = (HttpWebResponse) httpReq.GetResponse(); 
                Stream respStream = httpResp.GetResponseStream();
                StreamReader respStreamReader = new StreamReader(respStream,Encoding.UTF8);
                string line = respStreamReader.ReadLine();
                line = respStreamReader.ReadLine();
                while (true)
                {
                    line = respStreamReader.ReadLine();
                    if (line == null)
                    {
                        break;
                    }

                    string []cols = line.Split('\t');
                    GateServerInfo info;
                    info.serverID = uint.Parse(cols[0]);
                    info.IPPort = cols[5];
                    gateips.Add(info);
                    Console.WriteLine("Load gate {0} {1}", info.serverID, info.IPPort);
                }

                respStream.Close();
                return true;
            }

            public string GetGate(uint id)
            {
                for (int i = 0; i < gateips.Count; ++i)
                {
                    if (gateips[i].serverID == id)
                    {
                        return gateips[i].IPPort;
                    }
                }

                return null;
            }
            public void FillGate(KKSG.QueryGateRes roRes)
            {
                for (int i = 0; i < roRes.allservers.Count; ++i)
                {
                    GateServerInfo info;
                    info.serverID = (uint)roRes.allservers[i].serverid;
                    info.IPPort = roRes.allservers[i].ip + ":"+roRes.allservers[i].port;
                    gateips.Add(info);
                    Console.WriteLine("Load gate {0} {1}", info.serverID, info.IPPort);
                }

            }
        }

        static void Main(string[] args)
        {
            Conf config = new Conf();
            OptGet opt = new OptGet();
            opt.AddOptHandler("ip", config.HandleIP);
            opt.AddOptHandler("port", config.HandlePort);
            opt.AddOptHandler("acc", config.HandleAccount);
            opt.AddOptHandler("sv", config.HandleServerID);
            opt.AddOptHandler("s", config.HandleScript);
            opt.AddOptHandler("single", config.HandleSingle);
            opt.AddOptHandler("accs", config.HandleAccountStart);
            opt.AddOptHandler("acce", config.HandleAccountEnd);
            opt.AddOptHandler("accp", config.HandleAccountPrefix);
            opt.AddOptHandler("url", config.HandlerURL);
            if (!opt.Parse(args))
            {
                Console.WriteLine("RobotTest.exe " + opt.Help());
                return;
            }

            PtcRegister.RegistProtocol();
            handler = new GameProtocolHandler();
            mgr = new ConnectionMgr();

            int accountIndex = 0;
            if (config.single)
            {
                DoSingleTest(config);
            }
            else
            {
                accountIndex = config.accountStart;
            }

            DateTime lastConnectTime = DateTime.Now;
            DateTime lastTime = DateTime.Now;
            DateTime lastTimerTime = DateTime.Now;
            while (true)
            {
                if (DateTime.Now.Subtract(lastTime).TotalSeconds > 10)
                {
                    lastTime = DateTime.Now;
                    Console.WriteLine("" + ClientState.LoginRoleCount + " client is login");
                    Console.WriteLine("connection size:" + mgr.Count);

                    foreach (var x in TeamBattleTask.Total)
                    {
                        /*if (x.Key != TeamState.NULL_STATE &&
                            x.Key != TeamState.TEAM_BATTLE_END)*/
                        //if (x.Key != TeamState.NULL_STATE)
                        {
                            Console.WriteLine("=> {0} = {1}", x.Key.ToString(), x.Value);
                        }
                    }
                }

                //float deltatime = Time.time - lastTimerTime;

                if (DateTime.Now.Subtract(lastTimerTime).Milliseconds > 50)
                {
                    XTimerMgr.singleton.Update(DateTime.Now.Subtract(lastTimerTime).Milliseconds * 0.001f);
                    lastTimerTime = DateTime.Now;
                }

                if (!config.single)
                {
                    if (accountIndex < config.accountEnd)
                    {
                        if (DateTime.Now.Subtract(lastConnectTime).TotalMilliseconds > 100)
                        {
                            lastConnectTime = DateTime.Now;
                            DoConnect(config, accountIndex);
                            ++accountIndex;
                        }
                    }
                }
                

                mgr.ProcessMsg();
                mgr.Update();
            }
        }

        private static void DoConnect(Conf config, int i)
        {
            IPEndPoint ep = Connection.MakeEP(config.ip, config.port);

            Connection conn = new Connection();
            conn.mgr = mgr;
            conn.state.Account = config.accountPrefix + i.ToString();
            conn.state.Script = config.script;
            conn.state.ServerID = config.serverID;
            Console.WriteLine("Use account: " + conn.state.Account);
            conn.handler = handler;
            conn.Connect(ep);
            mgr.AddConnection(conn);
        }

        private static void DoSingleTest(Conf config)
        {
            IPEndPoint ep = Connection.MakeEP(config.ip, config.port);

            Connection conn = new Connection();
            conn.mgr = mgr;
            conn.state.Account = config.account;
            conn.state.Script = config.script;
            conn.state.ServerID = config.serverID;
            conn.handler = handler;
            conn.Connect(ep);
            mgr.AddConnection(conn);

            Console.WriteLine("Use account: " + conn.state.Account);
        }
    }
}
