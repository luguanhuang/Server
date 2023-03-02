using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/9/7 16:59:10

namespace XMainClient
{
	using ArgDataType = KKSG.FuseEquipArg;
    using ResDataType = KKSG.FuseEquipRes;

    class RpcC2G_FuseEquip : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2G_FuseEquip()
        {
        }

        public override uint GetRpcType()
        {
            return 56006;
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
            Process_RpcC2G_FuseEquip.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_FuseEquip.OnTimeout(oArg);
        }
    }
}
