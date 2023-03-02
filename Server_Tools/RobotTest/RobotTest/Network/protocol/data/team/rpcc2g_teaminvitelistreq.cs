using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/7/6 10:28:07

namespace XMainClient
{
	using ArgDataType = KKSG.TeamInviteArg;
    using ResDataType = KKSG.TeamInviteRes;

    class RpcC2G_TeamInviteListReq : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2G_TeamInviteListReq()
        {
        }

        public override uint GetRpcType()
        {
            return 11403;
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
            Process_RpcC2G_TeamInviteListReq.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_TeamInviteListReq.OnTimeout(oArg);
        }
    }
}
