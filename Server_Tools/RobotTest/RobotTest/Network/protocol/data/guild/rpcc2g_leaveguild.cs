using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2015/10/1 11:42:21

namespace XMainClient
{
	using ArgDataType = KKSG.LeaveGuildArg;
    using ResDataType = KKSG.LeaveGuildRes;

    class RpcC2G_LeaveGuild : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2G_LeaveGuild()
        {
        }

        public override uint GetRpcType()
        {
            return 37926;
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
            Process_RpcC2G_LeaveGuild.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_LeaveGuild.OnTimeout(oArg);
        }
    }
}
