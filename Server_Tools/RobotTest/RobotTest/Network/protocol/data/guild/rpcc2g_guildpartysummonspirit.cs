using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/10/10 10:23:06

namespace XMainClient
{
	using ArgDataType = KKSG.GuildPartySummonSpiritArg;
    using ResDataType = KKSG.GuildPartySummonSpiritRes;

    class RpcC2G_GuildPartySummonSpirit : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2G_GuildPartySummonSpirit()
        {
        }

        public override uint GetRpcType()
        {
            return 42269;
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
            Process_RpcC2G_GuildPartySummonSpirit.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_GuildPartySummonSpirit.OnTimeout(oArg);
        }
    }
}
