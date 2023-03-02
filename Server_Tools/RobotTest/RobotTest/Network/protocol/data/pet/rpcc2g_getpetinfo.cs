using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/9/14 17:17:55

namespace XMainClient
{
	using ArgDataType = KKSG.GetPetInfoArg;
    using ResDataType = KKSG.GetPetInfoRes;

    class RpcC2G_GetPetInfo : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2G_GetPetInfo()
        {
        }

        public override uint GetRpcType()
        {
            return 63259;
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
            Process_RpcC2G_GetPetInfo.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_GetPetInfo.OnTimeout(oArg);
        }
    }
}
