using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/8/1 0:23:38

namespace XMainClient
{
	using ArgDataType = KKSG.StartWeddingCarArg;
    using ResDataType = KKSG.StartWeddingCarRes;

    class RpcC2M_StartWeddingCar : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2M_StartWeddingCar()
        {
        }

        public override uint GetRpcType()
        {
            return 26388;
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
            Process_RpcC2M_StartWeddingCar.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2M_StartWeddingCar.OnTimeout(oArg);
        }
    }
}
