using System;
using System.Collections.Generic;
using System.Text;
using System.IO;

namespace XMainClient
{
    public abstract class Protocol
    {
        public static Dictionary<uint, Protocol> sm_RegistProtocol = new Dictionary<uint, Protocol>();

        public Connection conn;
        public virtual uint GetProtoType() { return 0; }

        public void SerializeWithHead(MemoryStream stream)
        {
            long begin = stream.Position;

            ProtocolHead head = new ProtocolHead();
            head.type = GetProtoType();
            head.flag = (uint)PtcHeadFlag.PTC_TYPE_FLAG;
            head.Serialize(stream);

            Serialize(stream);

            long position = stream.Position;
            uint length = (uint)(position - begin - sizeof(int));
            stream.Position = begin;
            stream.Write(BitConverter.GetBytes(length), 0, sizeof(UInt32));
            stream.Position = position;
        }

        public abstract void Serialize(MemoryStream stream);
        public abstract void DeSerialize(MemoryStream stream);


        public static Protocol GetProtocol(uint dwType)
        {
            if (sm_RegistProtocol.ContainsKey(dwType))
            {
                Protocol protocol;
                sm_RegistProtocol.TryGetValue(dwType, out protocol);
                return protocol;
            }
            else
            {
                return null;
            }
        }

        public static bool RegistProtocol(Protocol protocol)
        {
            if (sm_RegistProtocol.ContainsKey(protocol.GetProtoType()))
            {
                return false;
            }

            sm_RegistProtocol.Add(protocol.GetProtoType(), protocol);
            return true;
        }

        public virtual void Process()
        {

        }
    }
}
