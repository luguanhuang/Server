using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/4/10 14:49:54

namespace XMainClient
{
	using ArgDataType = KKSG.WorldBossGuildAddAttrArg;
    using ResDataType = KKSG.WorldBossGuildAddAttrRes;

    class RpcC2M_WorldBossGuildAddAttr : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2M_WorldBossGuildAddAttr()
        {
        }

        public override uint GetRpcType()
        {
            return 9805;
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
            Process_RpcC2M_WorldBossGuildAddAttr.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2M_WorldBossGuildAddAttr.OnTimeout(oArg);
        }
    }
}
