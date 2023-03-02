using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/7/19 23:53:59

namespace XMainClient
{
	using ArgDataType = KKSG.GuildArenaSimpleDeployArg;
    using ResDataType = KKSG.GuildArenaSimpleDeployRes;

    class RpcC2G_GuildArenaSimpleDeploy : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2G_GuildArenaSimpleDeploy()
        {
        }

        public override uint GetRpcType()
        {
            return 24284;
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
            Process_RpcC2G_GuildArenaSimpleDeploy.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_GuildArenaSimpleDeploy.OnTimeout(oArg);
        }
    }
}
