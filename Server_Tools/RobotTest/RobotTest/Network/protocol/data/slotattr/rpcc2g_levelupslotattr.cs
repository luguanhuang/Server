using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/3/22 15:54:35

namespace XMainClient
{
	using ArgDataType = KKSG.LevelUpSlotAttrArg;
    using ResDataType = KKSG.LevelUpSlotAttrRes;

    class RpcC2G_LevelUpSlotAttr : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2G_LevelUpSlotAttr()
        {
        }

        public override uint GetRpcType()
        {
            return 62918;
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
            Process_RpcC2G_LevelUpSlotAttr.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_LevelUpSlotAttr.OnTimeout(oArg);
        }
    }
}
