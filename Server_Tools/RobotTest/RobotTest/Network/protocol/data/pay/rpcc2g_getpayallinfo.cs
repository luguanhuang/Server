using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/8/16 20:18:45

namespace XMainClient
{
	using ArgDataType = KKSG.GetPayAllInfoArg;
    using ResDataType = KKSG.GetPayAllInfoRes;

    class RpcC2G_GetPayAllInfo : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2G_GetPayAllInfo()
        {
        }

        public override uint GetRpcType()
        {
            return 41260;
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
            Process_RpcC2G_GetPayAllInfo.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_GetPayAllInfo.OnTimeout(oArg);
        }
    }
}
