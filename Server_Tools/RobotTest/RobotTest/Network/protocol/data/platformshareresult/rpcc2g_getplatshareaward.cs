using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/5/31 21:54:00

namespace XMainClient
{
	using ArgDataType = KKSG.GetPlatShareAwardArg;
    using ResDataType = KKSG.GetPlatShareAwardRes;

    class RpcC2G_GetPlatShareAward : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2G_GetPlatShareAward()
        {
        }

        public override uint GetRpcType()
        {
            return 26922;
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
            Process_RpcC2G_GetPlatShareAward.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_GetPlatShareAward.OnTimeout(oArg);
        }
    }
}
