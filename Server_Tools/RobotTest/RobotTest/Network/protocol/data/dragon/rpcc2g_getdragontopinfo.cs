using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/7/4 17:55:37

namespace XMainClient
{
	using ArgDataType = KKSG.GetDragonTopInfoArg;
    using ResDataType = KKSG.GetDragonTopInfoRes;

    class RpcC2G_GetDragonTopInfo : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2G_GetDragonTopInfo()
        {
        }

        public override uint GetRpcType()
        {
            return 7973;
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
            Process_RpcC2G_GetDragonTopInfo.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_GetDragonTopInfo.OnTimeout(oArg);
        }
    }
}
