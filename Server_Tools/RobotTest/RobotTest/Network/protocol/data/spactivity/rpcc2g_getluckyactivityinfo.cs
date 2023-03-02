using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/9/12 11:13:18

namespace XMainClient
{
	using ArgDataType = KKSG.GetLuckyActivityInfoArg;
    using ResDataType = KKSG.GetLuckyActivityInfoRes;

    class RpcC2G_GetLuckyActivityInfo : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2G_GetLuckyActivityInfo()
        {
        }

        public override uint GetRpcType()
        {
            return 384;
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
            Process_RpcC2G_GetLuckyActivityInfo.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_GetLuckyActivityInfo.OnTimeout(oArg);
        }
    }
}
