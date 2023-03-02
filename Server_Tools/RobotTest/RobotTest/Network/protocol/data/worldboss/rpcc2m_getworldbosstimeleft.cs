using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/8/9 0:43:42

namespace XMainClient
{
	using ArgDataType = KKSG.GetWorldBossTimeLeftArg;
    using ResDataType = KKSG.GetWorldBossTimeLeftRes;

    class RpcC2M_GetWorldBossTimeLeft : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2M_GetWorldBossTimeLeft()
        {
        }

        public override uint GetRpcType()
        {
            return 23195;
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
            Process_RpcC2M_GetWorldBossTimeLeft.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2M_GetWorldBossTimeLeft.OnTimeout(oArg);
        }
    }
}
