using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2015/11/6 17:02:54

namespace XMainClient
{
	using ArgDataType = KKSG.ChatArg;
    using ResDataType = KKSG.ChatRes;

    class RpcC2G_Chat : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2G_Chat()
        {
        }

        public override uint GetRpcType()
        {
            return 39481;
        }

        public override void Serialize(MemoryStream stream)
        {
            Serializer.Serialize(stream, oArg);
        }

        public override void DeSerialize(MemoryStream stream)
        {
            oRes = Serializer.Deserialize<ResDataType>(stream);
        }

        public override void Process()
        {
            base.Process();
            Process_RpcC2G_Chat.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_Chat.OnTimeout(oArg);
        }
    }
}
