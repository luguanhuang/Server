using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/1/21 16:36:01

namespace XMainClient
{
	using ArgDataType = KKSG.SetRoleConfigReq;
    using ResDataType = KKSG.SetRoleConfigRes;

    class RpcC2G_SetRoleConfig : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2G_SetRoleConfig()
        {
        }

        public override uint GetRpcType()
        {
            return 35306;
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
            Process_RpcC2G_SetRoleConfig.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_SetRoleConfig.OnTimeout(oArg);
        }
    }
}
