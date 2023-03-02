using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/7/15 20:17:44

namespace XMainClient
{
	using ArgDataType = KKSG.ResetTowerArg;
    using ResDataType = KKSG.ResetTowerRes;

    class RpcC2G_ResetTower : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2G_ResetTower()
        {
        }

        public override uint GetRpcType()
        {
            return 8570;
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
            Process_RpcC2G_ResetTower.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_ResetTower.OnTimeout(oArg);
        }
    }
}
