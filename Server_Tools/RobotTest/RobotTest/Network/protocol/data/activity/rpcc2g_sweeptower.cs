using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/7/15 20:07:40

namespace XMainClient
{
	using ArgDataType = KKSG.SweepTowerArg;
    using ResDataType = KKSG.SweepTowerRes;

    class RpcC2G_SweepTower : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2G_SweepTower()
        {
        }

        public override uint GetRpcType()
        {
            return 39381;
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
            Process_RpcC2G_SweepTower.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_SweepTower.OnTimeout(oArg);
        }
    }
}
