using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/8/8 10:58:21

namespace XMainClient
{
	using ArgDataType = KKSG.GetWorldBossStateArg;
    using ResDataType = KKSG.GetWorldBossStateRes;

    class RpcC2M_GetWorldBossNew : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2M_GetWorldBossNew()
        {
        }

        public override uint GetRpcType()
        {
            return 34090;
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
            Process_RpcC2M_GetWorldBossNew.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2M_GetWorldBossNew.OnTimeout(oArg);
        }
    }
}
