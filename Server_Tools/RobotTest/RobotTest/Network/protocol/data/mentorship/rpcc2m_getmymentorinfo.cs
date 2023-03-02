using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/12/9 20:55:38

namespace XMainClient
{
	using ArgDataType = KKSG.GetMyMentorInfoArg;
    using ResDataType = KKSG.GetMyMentorInfoRes;

    class RpcC2M_GetMyMentorInfo : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2M_GetMyMentorInfo()
        {
        }

        public override uint GetRpcType()
        {
            return 8287;
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
            Process_RpcC2M_GetMyMentorInfo.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2M_GetMyMentorInfo.OnTimeout(oArg);
        }
    }
}
