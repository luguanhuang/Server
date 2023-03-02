using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/9/22 11:43:01

namespace XMainClient
{
	using ArgDataType = KKSG.GetOtherGuildBriefArg;
    using ResDataType = KKSG.GetOtherGuildBriefRes;

    class RpcC2M_GetOtherGuildBriefNew : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2M_GetOtherGuildBriefNew()
        {
        }

        public override uint GetRpcType()
        {
            return 16797;
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
            Process_RpcC2M_GetOtherGuildBriefNew.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2M_GetOtherGuildBriefNew.OnTimeout(oArg);
        }
    }
}
