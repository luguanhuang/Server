using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/10/5 10:30:13

namespace XMainClient
{
	using ArgDataType = KKSG.StartPlantArg;
    using ResDataType = KKSG.StartPlantRes;

    class RpcC2M_StartPlant : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2M_StartPlant()
        {
        }

        public override uint GetRpcType()
        {
            return 2834;
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
            Process_RpcC2M_StartPlant.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2M_StartPlant.OnTimeout(oArg);
        }
    }
}
