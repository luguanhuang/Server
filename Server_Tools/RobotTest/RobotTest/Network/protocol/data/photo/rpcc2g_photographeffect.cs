using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/12/14 21:43:55

namespace XMainClient
{
	using ArgDataType = KKSG.PhotographEffectArg;
    using ResDataType = KKSG.PhotographEffect;

    class RpcC2G_PhotographEffect : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2G_PhotographEffect()
        {
        }

        public override uint GetRpcType()
        {
            return 14666;
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
            Process_RpcC2G_PhotographEffect.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_PhotographEffect.OnTimeout(oArg);
        }
    }
}
