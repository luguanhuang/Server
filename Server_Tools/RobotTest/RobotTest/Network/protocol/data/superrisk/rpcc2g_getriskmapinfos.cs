using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/9/5 16:04:20

namespace XMainClient
{
	using ArgDataType = KKSG.GetRiskMapInfosArg;
    using ResDataType = KKSG.GetRiskMapInfosRes;

    class RpcC2G_GetRiskMapInfos : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2G_GetRiskMapInfos()
        {
        }

        public override uint GetRpcType()
        {
            return 11628;
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
            Process_RpcC2G_GetRiskMapInfos.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_GetRiskMapInfos.OnTimeout(oArg);
        }
    }
}
