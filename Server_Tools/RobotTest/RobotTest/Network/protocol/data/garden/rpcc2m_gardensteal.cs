using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/10/24 12:28:04

namespace XMainClient
{
	using ArgDataType = KKSG.GardenStealArg;
    using ResDataType = KKSG.GardenStealRes;

    class RpcC2M_GardenSteal : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2M_GardenSteal()
        {
        }

        public override uint GetRpcType()
        {
            return 12696;
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
            Process_RpcC2M_GardenSteal.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2M_GardenSteal.OnTimeout(oArg);
        }
    }
}
