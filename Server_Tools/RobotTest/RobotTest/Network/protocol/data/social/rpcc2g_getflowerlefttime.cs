using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/6/25 15:46:04

namespace XMainClient
{
	using ArgDataType = KKSG.GetFlowerLeftTimeArg;
    using ResDataType = KKSG.GetFlowerLeftTimeRes;

    class RpcC2G_GetFlowerLeftTime : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2G_GetFlowerLeftTime()
        {
        }

        public override uint GetRpcType()
        {
            return 26834;
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
            Process_RpcC2G_GetFlowerLeftTime.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_GetFlowerLeftTime.OnTimeout(oArg);
        }
    }
}
