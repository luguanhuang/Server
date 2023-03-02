using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/9/6 16:13:07

namespace XMainClient
{
	using ArgDataType = KKSG.TakePartnerChestArg;
    using ResDataType = KKSG.TakePartnerChestRes;

    class RpcC2G_TakeDragonGuildChest : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2G_TakeDragonGuildChest()
        {
        }

        public override uint GetRpcType()
        {
            return 38031;
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
            Process_RpcC2G_TakeDragonGuildChest.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_TakeDragonGuildChest.OnTimeout(oArg);
        }
    }
}
