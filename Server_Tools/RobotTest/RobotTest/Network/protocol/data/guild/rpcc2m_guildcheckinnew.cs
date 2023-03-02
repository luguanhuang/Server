using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/9/16 21:00:56

namespace XMainClient
{
	using ArgDataType = KKSG.GuildCheckinArg;
    using ResDataType = KKSG.GuildCheckinRes;

    class RpcC2M_GuildCheckinNew : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2M_GuildCheckinNew()
        {
        }

        public override uint GetRpcType()
        {
            return 5584;
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
            Process_RpcC2M_GuildCheckinNew.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2M_GuildCheckinNew.OnTimeout(oArg);
        }
    }
}
