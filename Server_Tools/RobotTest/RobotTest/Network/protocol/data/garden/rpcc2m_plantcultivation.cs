using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/10/5 10:32:43

namespace XMainClient
{
	using ArgDataType = KKSG.PlantCultivationArg;
    using ResDataType = KKSG.PlantCultivationRes;

    class RpcC2M_PlantCultivation : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2M_PlantCultivation()
        {
        }

        public override uint GetRpcType()
        {
            return 61295;
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
            Process_RpcC2M_PlantCultivation.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2M_PlantCultivation.OnTimeout(oArg);
        }
    }
}
