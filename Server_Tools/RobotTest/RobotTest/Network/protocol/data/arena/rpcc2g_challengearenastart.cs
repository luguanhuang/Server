using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2015/5/27 14:20:40

namespace XMainClient
{
	using ArgDataType = KKSG.ChallengeArenaStartArg;
    using ResDataType = KKSG.ChallengeArenaStartRes;

    class RpcC2G_ChallengeArenaStart : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2G_ChallengeArenaStart()
        {
        }

        public override uint GetRpcType()
        {
            return 26556;
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
            Process_RpcC2G_ChallengeArenaStart.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_ChallengeArenaStart.OnTimeout(oArg);
        }
    }
}
