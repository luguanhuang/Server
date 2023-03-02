using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2015/7/18 15:10:19

namespace XMainClient
{
	using ArgDataType = KKSG.GetWorldBossStateArg;
    using ResDataType = KKSG.GetWorldBossStateRes;

    class RpcC2G_GetWorldBossState : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2G_GetWorldBossState()
        {
        }

        public override uint GetRpcType()
        {
            return 54244;
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
            Process_RpcC2G_GetWorldBossState.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_GetWorldBossState.OnTimeout(oArg);
        }
    }
}
