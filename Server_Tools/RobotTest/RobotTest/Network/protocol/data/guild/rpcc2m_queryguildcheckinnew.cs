using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/9/16 21:06:11

namespace XMainClient
{
	using ArgDataType = KKSG.QueryGuildCheckinArg;
    using ResDataType = KKSG.QueryGuildCheckinRes;

    class RpcC2M_QueryGuildCheckinNew : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2M_QueryGuildCheckinNew()
        {
        }

        public override uint GetRpcType()
        {
            return 56433;
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
            Process_RpcC2M_QueryGuildCheckinNew.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2M_QueryGuildCheckinNew.OnTimeout(oArg);
        }
    }
}
