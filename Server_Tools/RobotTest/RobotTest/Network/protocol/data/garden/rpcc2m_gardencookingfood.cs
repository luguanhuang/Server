using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/10/10 14:15:05

namespace XMainClient
{
	using ArgDataType = KKSG.GardenCookingFoodArg;
    using ResDataType = KKSG.GardenCookingFoodRes;

    class RpcC2M_GardenCookingFood : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2M_GardenCookingFood()
        {
        }

        public override uint GetRpcType()
        {
            return 31406;
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
            Process_RpcC2M_GardenCookingFood.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2M_GardenCookingFood.OnTimeout(oArg);
        }
    }
}
