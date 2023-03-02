using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Net.Sockets;
using System.Collections;

namespace XMainClient
{
    public class ConnectionMgr
    {
        public static Connection CurrentConnection;
        public static int errlistsize = 0;

        List<Connection> connections;
        Dictionary<Socket, Connection> sockToconn;
        ArrayList readList;
        ArrayList writeList;
        ArrayList errList;

        public ConnectionMgr()
        {
            connections = new List<Connection>();
            sockToconn = new Dictionary<Socket, Connection>();
            readList = new ArrayList();
            writeList = new ArrayList();
            errList = new ArrayList();
        }

        public int Count
        {
            get { return connections.Count; }
        }

        public void AddConnection(Connection conn)
        {
            connections.Add(conn);
            sockToconn.Add(conn.socket, conn);
        }

        public void RemoveConnection(Connection conn)
        {
            connections.Remove(conn);
            sockToconn.Remove(conn.socket);
        }

        public Connection FindConnection(Socket sock)
        {
            Connection conn;
            if (sockToconn.TryGetValue(sock, out conn))
            {
                return conn;
            }
            return null;
        }

        public void Update()
        {
            for (int i = 0; i < connections.Count; ++i)
            {
                connections[i].state.Update();
            }
        }

        public void RemoveSocket(Socket old)
        {
            sockToconn.Remove(old);
        }

        public void ProcessMsg()
        {
            if (connections.Count == 0)
            {
                return;
            }

            readList.Clear();   
            errList.Clear();

            for (int i = 0; i < connections.Count; ++i)
            {
                readList.Add(connections[i].socket);
                errList.Add(connections[i].socket);
            }

            Socket.Select(readList, writeList, errList, 10);
            
            //Console.WriteLine("errlist size: " + errList.Count);
            //Console.WriteLine("readlist size: " + readList.Count);
            //Console.WriteLine("writelist size: " + writeList.Count);
            errlistsize += errList.Count;
            if(errlistsize > 0)
                Console.WriteLine("errorlist size: " + errlistsize);

            for (int i = 0; i < errList.Count; ++i)
            {
                CurrentConnection = FindConnection(errList[i] as Socket);
                if (CurrentConnection != null)
                {
                    CurrentConnection.Close();
                }
            }

            for (int i = 0; i < readList.Count; ++i)
            {
                CurrentConnection = FindConnection(readList[i] as Socket);
                if (CurrentConnection != null)
                {
                    CurrentConnection.OnRead();
                }
            }

            for (int i = connections.Count-1; i >= 0; --i)
            {
                CurrentConnection = connections[i];
                if (CurrentConnection.IsClosed && CurrentConnection.state.State >= StateDef.RecieveRoleData)
                {
                    -- ClientState.LoginRoleCount;
                    if (CurrentConnection.GetTask().GetTaskType() == TaskType.TYPE_TEAMTASK)
                    {
                        TeamBattleTask task = ((TeamBattleTask)CurrentConnection.state.GetCurrentTask());
                        -- TeamBattleTask.Total[task.GetState()];
                    }
                    CurrentConnection.Cleanup();
                    connections.RemoveAt(i);
                    Console.WriteLine("remove connection, size:" + connections.Count);
                }
            }

            CurrentConnection = null;
        }
    }
}
