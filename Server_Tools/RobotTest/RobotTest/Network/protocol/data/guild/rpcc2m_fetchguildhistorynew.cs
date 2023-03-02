using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/9/20 14:22:23

namespace XMainClient
{
	using ArgDataType = KKSG.GuildHistoryArg;
    using ResDataType = KKSG.GuildHistoryRes;

    class RpcC2M_FetchGuildHistoryNew : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2M_FetchGuildHistoryNew()
        {
        }

        public override uint GetRpcType()
        {
            return 26284;
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
            Process_RpcC2M_FetchGuildHistoryNew.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2M_FetchGuildHistoryNew.OnTimeout(oArg);
        }
    }
}
