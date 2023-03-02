using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/11/3 2:19:50

namespace XMainClient
{
	using ArgDataType = KKSG.ReturnToSelectRoleArg;
    using ResDataType = KKSG.ReturnToSelectRoleRes;

    class RpcC2N_ReturnToSelectRole : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2N_ReturnToSelectRole()
        {
        }

        public override uint GetRpcType()
        {
            return 25477;
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
            Process_RpcC2N_ReturnToSelectRole.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2N_ReturnToSelectRole.OnTimeout(oArg);
        }
    }
}
