using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/11/19 10:14:54

namespace XMainClient
{
	using ArgDataType = KKSG.ReqGuildInheritNumArg;
    using ResDataType = KKSG.ReqGuildInheritNum;

    class RpcC2G_ReqGuildInheritNum : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2G_ReqGuildInheritNum()
        {
        }

        public override uint GetRpcType()
        {
            return 6777;
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
            Process_RpcC2G_ReqGuildInheritNum.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_ReqGuildInheritNum.OnTimeout(oArg);
        }
    }
}
