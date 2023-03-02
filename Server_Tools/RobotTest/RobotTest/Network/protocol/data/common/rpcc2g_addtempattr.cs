using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2015/7/21 16:18:27

namespace XMainClient
{
	using ArgDataType = KKSG.AddTempAttrArg;
    using ResDataType = KKSG.AddTempAttrRes;

    class RpcC2G_AddTempAttr : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2G_AddTempAttr()
        {
        }

        public override uint GetRpcType()
        {
            return 22021;
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
            Process_RpcC2G_AddTempAttr.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_AddTempAttr.OnTimeout(oArg);
        }
    }
}
