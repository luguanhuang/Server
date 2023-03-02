using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/7/28 10:11:38

namespace XMainClient
{
	using ArgDataType = KKSG.CheckRoleBeforePayArg;
    using ResDataType = KKSG.CheckRoleBeforePayRes;

    class RpcC2M_CheckRoleBeforePay : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2M_CheckRoleBeforePay()
        {
        }

        public override uint GetRpcType()
        {
            return 56255;
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
            Process_RpcC2M_CheckRoleBeforePay.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2M_CheckRoleBeforePay.OnTimeout(oArg);
        }
    }
}
