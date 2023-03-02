using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/10/10 15:45:06

namespace XMainClient
{
	using ArgDataType = KKSG.GetGuildBindInfoReq;
    using ResDataType = KKSG.GetGuildBindInfoRes;

    class RpcC2M_GetGuildBindInfo : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2M_GetGuildBindInfo()
        {
        }

        public override uint GetRpcType()
        {
            return 62512;
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
            Process_RpcC2M_GetGuildBindInfo.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2M_GetGuildBindInfo.OnTimeout(oArg);
        }
    }
}
