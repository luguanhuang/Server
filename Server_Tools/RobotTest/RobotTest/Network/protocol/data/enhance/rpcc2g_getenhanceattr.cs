using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/7/6 16:17:50

namespace XMainClient
{
	using ArgDataType = KKSG.GetEnhanceAttrArg;
    using ResDataType = KKSG.GetEnhanceAttrRes;

    class RpcC2G_GetEnhanceAttr : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2G_GetEnhanceAttr()
        {
        }

        public override uint GetRpcType()
        {
            return 23396;
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
            Process_RpcC2G_GetEnhanceAttr.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_GetEnhanceAttr.OnTimeout(oArg);
        }
    }
}
