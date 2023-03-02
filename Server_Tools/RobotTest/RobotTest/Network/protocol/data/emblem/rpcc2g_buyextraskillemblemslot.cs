using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/5/9 16:28:24

namespace XMainClient
{
	using ArgDataType = KKSG.BuyExtraSkillEmblemSlotArg;
    using ResDataType = KKSG.BuyExtraSkillEmblemSlotRes;

    class RpcC2G_BuyExtraSkillEmblemSlot : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2G_BuyExtraSkillEmblemSlot()
        {
        }

        public override uint GetRpcType()
        {
            return 17851;
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
            Process_RpcC2G_BuyExtraSkillEmblemSlot.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_BuyExtraSkillEmblemSlot.OnTimeout(oArg);
        }
    }
}
