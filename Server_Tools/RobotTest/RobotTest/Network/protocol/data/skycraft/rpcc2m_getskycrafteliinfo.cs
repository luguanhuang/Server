using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/4/11 15:17:27

namespace XMainClient
{
	using ArgDataType = KKSG.GetSkyCraftEliInfoArg;
    using ResDataType = KKSG.GetSkyCraftEliInfoRes;

    class RpcC2M_GetSkyCraftEliInfo : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2M_GetSkyCraftEliInfo()
        {
        }

        public override uint GetRpcType()
        {
            return 41103;
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
            Process_RpcC2M_GetSkyCraftEliInfo.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2M_GetSkyCraftEliInfo.OnTimeout(oArg);
        }
    }
}
