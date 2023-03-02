using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Net.Sockets;
using System.Net;
using System.IO;
using System.Runtime.InteropServices;

namespace XMainClient
{
    public interface IProtocolHandler
    {
        void OnPacket(Connection conn, byte[] data);
    }

    class PrintProtocolHandler : IProtocolHandler
    {
        public void OnPacket(Connection conn, byte[] data)
        {
            Console.WriteLine("recv protocol len: " + data.Length);
        }
    }

    public class StopWatch
    {
        class StopWatchException : Exception
        {
            private StopState state;

            public StopWatchException(StopState e)
            {
                state = e;
            }

            public override string ToString()
            {
                return "Stop Watch State Error: " + state.ToString();
            }
        }

        [DllImport("kernel32.dll ")]
        static extern bool QueryPerformanceCounter(ref   long lpPerformanceCount);

        [DllImport("kernel32.dll")]
        static extern bool QueryPerformanceFrequency(ref long PerformanceFrequency);

        enum StopState
        {
            INIT,
            START,
            STOP,
        }
        
        static long Freq;
        long Begin;
        long End;
        StopState State;

        public StopWatch()
        {
            State = StopState.INIT;
        }

        static StopWatch()
        {
            QueryPerformanceFrequency(ref Freq);
        }

        public void Start()
        {
            State = StopState.START;
            QueryPerformanceCounter(ref Begin);
        }

        public void Stop()
        {
            State = StopState.STOP;
            QueryPerformanceCounter(ref End);
        }

        public double TimePassed()
        {
            if (State != StopState.STOP)
            {
                throw new StopWatchException(State);
            }

            double time = (End - Begin);
            time /= Freq;
            return time;
        }
    }

    public class Connection
    {
        private static int UUID = 1;
        public Socket socket;
        private bool closed;
        private int currLen;
        public int uuid;
        private byte[] buffer;

        public IProtocolHandler handler;
        public ClientState state;
        public ConnectionMgr mgr;

        public Connection()
        {
            uuid = UUID++;
            closed = false;
            socket = null;
            currLen = 0;
            state = new ClientState(this);
            buffer = new byte[10240];
        }

        public bool IsClosed
        {
            get { return closed; }
        }

        public static IPEndPoint MakeEP(string ip, int port)
        {
            IPAddress addr = IPAddress.Parse(ip);
            IPEndPoint ep = new IPEndPoint(addr, port);
            return ep;
        }

        public bool Connect(IPEndPoint ep)
        {
            try
            {
                socket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
                socket.Connect(ep);
                OnConnect();
                closed = false;
                return true;
            }
            catch (Exception ex)
            {
                Console.WriteLine("[" + state.Account + "] Connect Failed!" + ex.Message);
                closed = true;
                return false;
            }
        }

        public bool Reconnect(string ip, int port)
        {
            currLen = 0;
            mgr.RemoveConnection(this);
            socket.Close();
            socket = null;
            state.State = StateDef.Disconnect;
            bool ret = Connect(MakeEP(ip, port));
            if (ret)
            {
                mgr.AddConnection(this);
            }

            return ret;
        }

        private void OnConnect()
        {
            state.State = StateDef.Connected;
            //Console.WriteLine("OnConnected");
        }

        public Task GetTask()
        {
            return state.GetCurrentTask();
        }

        public void OnRead()
        {
            if (closed == true)
            {
                return;
            }

            try
            {
                int bytes = socket.Receive(buffer, currLen, buffer.Length - currLen, SocketFlags.None);
                if (bytes > 0)
                {
                    currLen += bytes;
                    //Console.WriteLine("read " + bytes + " bytes");
                    DetectPacket();
                }
                else
                {
                    closed = true;
                }
            }
            catch (System.Exception ex)
            {
                Console.WriteLine("[" + state.Account + "] Receive Failed!: " + ex.Message);
                closed = true;
            }
        }

        public void DetectPacket()
        {
            int pcount = 0;
            int offset = 0;
            int left = currLen;
            while (currLen > 4 + offset)
            {
                int packetLen = BitConverter.ToInt32(buffer, offset);
                if (currLen < 4 + offset + packetLen)
                {
                    break;
                }

                //Console.WriteLine("packet len [" + packetLen + "] from offset [" + offset + "] buff len " + currLen);
                byte[] data = new byte[packetLen+4];
                Array.Copy(buffer, offset, data, 0, packetLen+4);
                handler.OnPacket(this, data);
                ++pcount;
                offset += 4;
                offset += packetLen;
            }

            currLen -= offset;
            if (offset > 0 && currLen > 0)
            {
                Array.Copy(buffer, offset, buffer, 0, currLen);
            }

            //Console.WriteLine("packet " + pcount + " detect, left " + currLen);
        }



        public bool Send(byte[] buffer, long length)
        {
            try
            {
                if (closed == false)
                {
                    StopWatch watch = new StopWatch();
                    watch.Start();
                    int left = (int)length;
                    int off = 0;
                    while (left > 0)
                    {
                        int bytes = socket.Send(buffer, off, left, SocketFlags.None);
                        if (bytes < 0)
                        {
                            return false;
                        }

                        off += bytes;
                        left -= bytes;
                    }
                    watch.Stop();

                    double SendUseTime = watch.TimePassed()*1000;
                    if (SendUseTime > 10)
                    {
                        Console.WriteLine("Send Use Time {0:f2}ms", SendUseTime);
                    }
                    return true;
                }
            }
            catch(SocketException e)
            {
                Console.WriteLine(e.ToString());
            }
            catch (Exception e)
            {
                Console.WriteLine(e.ToString());
            }
            return false;
        }

        public void Close()
        {
            Console.WriteLine("close connection " + uuid);
            closed = true;
        }

        public void Cleanup()
        {
            Console.WriteLine("clean up connection " + uuid);
            socket.Close();
            buffer = null;
        }

        public bool Send(Protocol protocol)
        {
            bool temp = false;
            try
            {
                MemoryStream stream = new MemoryStream();
                protocol.SerializeWithHead(stream);
                temp = Send(stream.GetBuffer(), stream.Length);
            }
            catch (Exception e)
            {
                Console.WriteLine("error : " + e.StackTrace);
            }

            return temp;
        }

        public bool Send(Rpc rpc)
        {
            MemoryStream stream = new MemoryStream();
            rpc.OnSend();
            rpc.SerializeWithHead(stream);

            //Console.WriteLine("rpc: " + rpc.GetType() + "  " + string.Join(",",stream.GetBuffer().ToList()) + " ,len: " + stream.Length);
            return Send(stream.GetBuffer(), stream.Length);
        }
    }
}
