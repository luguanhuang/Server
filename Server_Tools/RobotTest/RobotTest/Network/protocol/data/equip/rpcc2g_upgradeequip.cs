using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/8/29 16:48:14

namespace XMainClient
{
	using ArgDataType = KKSG.UpgradeEquipArg;
    using ResDataType = KKSG.UpgradeEquipRes;

    class RpcC2G_UpgradeEquip : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2G_UpgradeEquip()
        {
        }

        public override uint GetRpcType()
        {
            return 32424;
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
            Process_RpcC2G_UpgradeEquip.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_UpgradeEquip.OnTimeout(oArg);
        }
    }
}
