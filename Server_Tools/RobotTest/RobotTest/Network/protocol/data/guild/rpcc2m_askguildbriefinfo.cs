using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/8/26 15:44:15

namespace XMainClient
{
	using ArgDataType = KKSG.GuildBriefArg;
    using ResDataType = KKSG.GuildBriefRes;

    class RpcC2M_AskGuildBriefInfo : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2M_AskGuildBriefInfo()
        {
        }

        public override uint GetRpcType()
        {
            return 53355;
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
            Process_RpcC2M_AskGuildBriefInfo.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2M_AskGuildBriefInfo.OnTimeout(oArg);
        }
    }
}
