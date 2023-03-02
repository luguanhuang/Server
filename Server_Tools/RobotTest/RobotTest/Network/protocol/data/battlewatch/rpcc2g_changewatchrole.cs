using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/7/11 16:33:38

namespace XMainClient
{
	using ArgDataType = KKSG.ChangeWatchRoleArg;
    using ResDataType = KKSG.ChangeWatchRoleRes;

    class RpcC2G_ChangeWatchRole : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2G_ChangeWatchRole()
        {
        }

        public override uint GetRpcType()
        {
            return 35369;
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
            Process_RpcC2G_ChangeWatchRole.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_ChangeWatchRole.OnTimeout(oArg);
        }
    }
}
