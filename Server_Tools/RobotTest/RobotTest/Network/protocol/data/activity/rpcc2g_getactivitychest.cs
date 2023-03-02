using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2015/6/27 12:23:39

namespace XMainClient
{
	using ArgDataType = KKSG.GetActivityChestArg;
    using ResDataType = KKSG.GetActivityChestRes;

    class RpcC2G_GetActivityChest : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2G_GetActivityChest()
        {
        }

        public override uint GetRpcType()
        {
            return 34363;
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
            Process_RpcC2G_GetActivityChest.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_GetActivityChest.OnTimeout(oArg);
        }
    }
}
