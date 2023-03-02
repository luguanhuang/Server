using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/4/11 14:28:48

namespace XMainClient
{
	using ArgDataType = KKSG.GetSkyCraftInfoArg;
    using ResDataType = KKSG.GetSkyCraftInfoRes;

    class RpcC2M_GetSkyCraftInfo : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2M_GetSkyCraftInfo()
        {
        }

        public override uint GetRpcType()
        {
            return 26199;
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
            Process_RpcC2M_GetSkyCraftInfo.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2M_GetSkyCraftInfo.OnTimeout(oArg);
        }
    }
}
