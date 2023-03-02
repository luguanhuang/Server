using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2015/6/30 10:44:12

namespace XMainClient
{
	using ArgDataType = KKSG.BuyJadeSlotArg;
    using ResDataType = KKSG.BuyJadeSlotRes;

    class RpcC2G_BuyJadeSlot : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2G_BuyJadeSlot()
        {
        }

        public override uint GetRpcType()
        {
            return 37813;
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
            Process_RpcC2G_BuyJadeSlot.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_BuyJadeSlot.OnTimeout(oArg);
        }
    }
}
