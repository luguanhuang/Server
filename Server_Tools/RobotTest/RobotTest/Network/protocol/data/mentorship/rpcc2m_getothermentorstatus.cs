using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/12/9 17:05:33

namespace XMainClient
{
	using ArgDataType = KKSG.GetOtherMentorStatusArg;
    using ResDataType = KKSG.GetOtherMentorStatusRes;

    class RpcC2M_GetOtherMentorStatus : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2M_GetOtherMentorStatus()
        {
        }

        public override uint GetRpcType()
        {
            return 4896;
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
            Process_RpcC2M_GetOtherMentorStatus.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2M_GetOtherMentorStatus.OnTimeout(oArg);
        }
    }
}
