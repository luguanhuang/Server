using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

namespace XMainClient
{
    // PtcHeadFlag 是用来填 ProtocolHead 的 dwFlag字段。
    // dwFlag 字段意义
    // 32个bit从高到低 31-24 23-16 15-8 7-0
    //  Protocol       0     0    0   00000000
    //  RPC Request    0     0    0   00000011
    //  RPC Reply      0     0    0   00000001
    //  Compress       0     0    0   00000100

    enum PtcHeadFlag
    {
        PTC_TYPE_FLAG = 0,
        RPC_TYPE_FLAG = 1,
        RPC_TYPE_REPLY = 1,
        RPC_TYPE_ISREQUEST_MASK = 2,
        RPC_TYPE_REQUEST = 3,
        RPC_TYPE_COMPRESS = 4,
    };

    class ProtocolHead
    {
        public uint len;
        public uint type;
        public uint flag;
        public uint tagID;
        public const uint MinSize = 12;

        public bool IsPtc { get { return !TestBit(flag, 0); } }
        public bool IsRpc { get { return TestBit(flag, 0); } }
        public bool IsRpcReply { get { return !TestBit(flag, 1); } }
        public bool IsRpcRequest { get { return TestBit(flag, 1); } }
        public bool IsCompressed { get { return TestBit(flag, 2); } }

        public bool TestBit(uint value, int bit)
        {
            return (value & (1 << bit)) != 0;
        }

        public ProtocolHead()
        {
            len = 0;
            type = 0;
            flag = 0;
        }

        public int Size
        {
            get
            {
                if (IsRpc)
                    return 16;
                else
                    return 12;
            }
        }

        public void Deserialize(byte[] bytes)
        {
            len = BitConverter.ToUInt32(bytes, 0);
            type = BitConverter.ToUInt32(bytes, 4);
            flag = BitConverter.ToUInt32(bytes, 8);

            if (IsRpc)
            {
                tagID = BitConverter.ToUInt32(bytes, 12);
            }
        }

        public void Serialize(MemoryStream stream)
        {
            stream.Write(BitConverter.GetBytes(len), 0, 4);
            stream.Write(BitConverter.GetBytes(type), 0, 4);
            stream.Write(BitConverter.GetBytes(flag), 0, 4);

            if (IsRpc)
            {
                stream.Write(BitConverter.GetBytes(tagID), 0, 4);
            }
        }
    }
}
