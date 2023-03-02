using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/1/10 10:36:59

namespace XMainClient
{
	using ArgDataType = KKSG.ForgeEquipArg;
    using ResDataType = KKSG.ForgeEquipRes;

    class RpcC2G_ForgeEquip : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2G_ForgeEquip()
        {
        }

        public override uint GetRpcType()
        {
            return 58244;
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
            Process_RpcC2G_ForgeEquip.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_ForgeEquip.OnTimeout(oArg);
        }
    }
}
