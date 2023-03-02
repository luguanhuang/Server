using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/7/28 17:09:02

namespace XMainClient
{
	using ArgDataType = KKSG.TakeMarriageChestArg;
    using ResDataType = KKSG.TakeMarriageChestRes;

    class RpcC2M_TakeMarriageChest : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2M_TakeMarriageChest()
        {
        }

        public override uint GetRpcType()
        {
            return 38713;
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
            Process_RpcC2M_TakeMarriageChest.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2M_TakeMarriageChest.OnTimeout(oArg);
        }
    }
}
