using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/9/6 10:43:57

namespace XMainClient
{
	using ArgDataType = KKSG.GuildMemberArg;
    using ResDataType = KKSG.GuildMemberRes;

    class RpcC2M_AskGuildMembers : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2M_AskGuildMembers()
        {
        }

        public override uint GetRpcType()
        {
            return 57958;
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
            Process_RpcC2M_AskGuildMembers.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2M_AskGuildMembers.OnTimeout(oArg);
        }
    }
}
