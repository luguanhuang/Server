using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/5/8 20:49:58

namespace XMainClient
{
	using ArgDataType = KKSG.EnchantActiveAttributeArg;
    using ResDataType = KKSG.EnchantActiveAttributeRes;

    class RpcC2G_EnchantActiveAttribute : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2G_EnchantActiveAttribute()
        {
        }

        public override uint GetRpcType()
        {
            return 19086;
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
            Process_RpcC2G_EnchantActiveAttribute.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_EnchantActiveAttribute.OnTimeout(oArg);
        }
    }
}
