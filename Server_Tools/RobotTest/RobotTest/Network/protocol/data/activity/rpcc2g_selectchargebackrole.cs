using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/10/26 12:00:50

namespace XMainClient
{
	using ArgDataType = KKSG.SelectChargeBackRoleArg;
    using ResDataType = KKSG.SelectChargeBackRoleRes;

    class RpcC2G_SelectChargeBackRole : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2G_SelectChargeBackRole()
        {
        }

        public override uint GetRpcType()
        {
            return 38792;
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
            Process_RpcC2G_SelectChargeBackRole.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_SelectChargeBackRole.OnTimeout(oArg);
        }
    }
}
