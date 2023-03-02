using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/6/25 16:14:12

namespace XMainClient
{
	using ArgDataType = KKSG.GetFlowerArg;
    using ResDataType = KKSG.GetFlowerRes;

    class RpcC2G_GetFlower : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2G_GetFlower()
        {
        }

        public override uint GetRpcType()
        {
            return 11473;
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
            Process_RpcC2G_GetFlower.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_GetFlower.OnTimeout(oArg);
        }
    }
}
