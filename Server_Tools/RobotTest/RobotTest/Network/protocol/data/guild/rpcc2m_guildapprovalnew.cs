using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/9/15 21:03:11

namespace XMainClient
{
	using ArgDataType = KKSG.GuildApprovalArg;
    using ResDataType = KKSG.GuildApprovalRes;

    class RpcC2M_GuildApprovalNew : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2M_GuildApprovalNew()
        {
        }

        public override uint GetRpcType()
        {
            return 28348;
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
            Process_RpcC2M_GuildApprovalNew.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2M_GuildApprovalNew.OnTimeout(oArg);
        }
    }
}
