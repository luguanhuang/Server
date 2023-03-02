using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/12/22 9:47:58

namespace XMainClient
{
	using ArgDataType = KKSG.TakePartnerChestArg;
    using ResDataType = KKSG.TakePartnerChestRes;

    class RpcC2G_TakePartnerChest : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2G_TakePartnerChest()
        {
        }

        public override uint GetRpcType()
        {
            return 42982;
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
            Process_RpcC2G_TakePartnerChest.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_TakePartnerChest.OnTimeout(oArg);
        }
    }
}
