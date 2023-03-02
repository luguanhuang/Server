using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using Ionic.Zlib;

namespace XMainClient
{
    class GameProtocolHandler : IProtocolHandler
    {
        public void OnPacket(Connection conn, byte[] data)
        {
            if (data.Length < ProtocolHead.MinSize)
            {
                return;
            }

            try
            {

                ProtocolHead head = new ProtocolHead();
                head.Deserialize(data);
                int size = head.Size;

                MemoryStream stream = new MemoryStream();
                if (head.IsCompressed)
                {
                    ZlibStream zOut = new ZlibStream(stream, CompressionMode.Decompress, true);
                    zOut.Write(data, size, data.Length - size);
                    zOut.Flush();
                    stream.Seek(0, SeekOrigin.Begin);
                }
                else
                {
                    stream = new MemoryStream(data, size, data.Length - size);
                }

                if (head.IsPtc)
                {
                    Protocol protocol = Protocol.GetProtocol(head.type);
                    if (protocol == null)
                    {
                        return;
                    }

                    //Console.WriteLine(protocol.ToString());
                    protocol.conn = conn;
                    protocol.DeSerialize(stream);
                    protocol.Process();
                }
                else
                {
                    Rpc rpc = Rpc.GetRpcByTag(head.tagID);
                    if (rpc == null)
                    {
                        return;
                    }

                    //Console.WriteLine(rpc.ToString());
                    rpc.conn = conn;
                    rpc.DeSerialize(stream);
                    rpc.Process();
                }
            }
            catch (Exception e)
            {
                Console.WriteLine(e.ToString());
            }
        }
    }
}
