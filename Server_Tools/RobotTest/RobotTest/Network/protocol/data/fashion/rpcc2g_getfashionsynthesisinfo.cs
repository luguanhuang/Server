using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/10/9 12:05:57

namespace XMainClient
{
	using ArgDataType = KKSG.FashionSynthesisInfoArg;
    using ResDataType = KKSG.FashionSynthesisInfoRes;

    class RpcC2G_GetFashionSynthesisInfo : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2G_GetFashionSynthesisInfo()
        {
        }

        public override uint GetRpcType()
        {
            return 26721;
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
            Process_RpcC2G_GetFashionSynthesisInfo.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_GetFashionSynthesisInfo.OnTimeout(oArg);
        }
    }
}
