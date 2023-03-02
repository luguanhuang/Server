using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/1/10 10:51:25

namespace XMainClient
{
	using ArgDataType = KKSG.EnchantEquipArg;
    using ResDataType = KKSG.EnchantEquipRes;

    class RpcC2G_EnchantEquip : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2G_EnchantEquip()
        {
        }

        public override uint GetRpcType()
        {
            return 55166;
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
            Process_RpcC2G_EnchantEquip.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_EnchantEquip.OnTimeout(oArg);
        }
    }
}
