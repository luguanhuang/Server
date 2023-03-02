using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Net;
using System.Net.Sockets;

namespace BuildTool
{

    class UDP
    {
        public Socket _socket;
        public IPEndPoint _peerpoint;

        const int PACKET_SIZE = 10240;

        public UDP(string host, short port)
        {
            _peerpoint = new IPEndPoint(IPAddress.Parse(host), port);
            _socket = new Socket(AddressFamily.InterNetwork, SocketType.Dgram, ProtocolType.Udp);
        }

        public void Bind(short port)
        {
            _socket.Bind(new IPEndPoint(IPAddress.Any, port));
        }

        public void Send(string data)
        {
            var send = Encoding.ASCII.GetBytes(data);
            int error = _socket.SendTo(send, send.Length, SocketFlags.None, _peerpoint);
        }

        public string Work()
        {
            byte[] data = new byte[PACKET_SIZE];
            EndPoint point = _peerpoint;
            int recv = _socket.ReceiveFrom(data, ref point);
            if (recv > 0)
            {
                var output = Encoding.ASCII.GetString(data, 0, recv);
                return output;
            }
            return "";
        }

        public void Stop()
        {
            _socket.Close();
        }
    }
}
