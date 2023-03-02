using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2015/9/23 16:12:41

namespace XMainClient
{
	using ArgDataType = KKSG.SendFlowerArg;
    using ResDataType = KKSG.SendFlowerRes;

    class RpcC2G_SendFlower : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2G_SendFlower()
        {
        }

        public override uint GetRpcType()
        {
            return 16310;
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
            Process_RpcC2G_SendFlower.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_SendFlower.OnTimeout(oArg);
        }
    }
}
