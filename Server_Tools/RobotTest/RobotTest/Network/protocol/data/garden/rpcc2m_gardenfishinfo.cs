using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/10/22 17:31:29

namespace XMainClient
{
	using ArgDataType = KKSG.GardenFishInfoArg;
    using ResDataType = KKSG.GardenFishInfoRes;

    class RpcC2M_GardenFishInfo : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2M_GardenFishInfo()
        {
        }

        public override uint GetRpcType()
        {
            return 10768;
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
            Process_RpcC2M_GardenFishInfo.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2M_GardenFishInfo.OnTimeout(oArg);
        }
    }
}
