using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/11/18 16:23:02

namespace XMainClient
{
	using ArgDataType = KKSG.ReqGuildInheritInfoArg;
    using ResDataType = KKSG.ReqGuildInheritInfoRes;

    class RpcC2M_ReqGuildInheritInfo : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2M_ReqGuildInheritInfo()
        {
        }

        public override uint GetRpcType()
        {
            return 7131;
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
            Process_RpcC2M_ReqGuildInheritInfo.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2M_ReqGuildInheritInfo.OnTimeout(oArg);
        }
    }
}
