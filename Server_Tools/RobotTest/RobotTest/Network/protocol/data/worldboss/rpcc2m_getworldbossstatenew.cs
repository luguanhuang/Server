using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/8/8 19:24:07

namespace XMainClient
{
	using ArgDataType = KKSG.GetWorldBossStateArg;
    using ResDataType = KKSG.GetWorldBossStateRes;

    class RpcC2M_GetWorldBossStateNew : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2M_GetWorldBossStateNew()
        {
        }

        public override uint GetRpcType()
        {
            return 17093;
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
            Process_RpcC2M_GetWorldBossStateNew.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2M_GetWorldBossStateNew.OnTimeout(oArg);
        }
    }
}
