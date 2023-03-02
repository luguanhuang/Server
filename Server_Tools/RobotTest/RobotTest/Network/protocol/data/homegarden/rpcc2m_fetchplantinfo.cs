using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/10/2 20:17:40

namespace XMainClient
{
	using ArgDataType = KKSG.FetchPlantInfoArg;
    using ResDataType = KKSG.FetchPlantInfoRes;

    class RpcC2M_FetchPlantInfo : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2M_FetchPlantInfo()
        {
        }

        public override uint GetRpcType()
        {
            return 606;
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
            Process_RpcC2M_FetchPlantInfo.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2M_FetchPlantInfo.OnTimeout(oArg);
        }
    }
}
