using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2015/6/4 15:41:44

namespace XMainClient
{
	using ArgDataType = KKSG.DecomposeEquipmentArg;
    using ResDataType = KKSG.DecomposeEquipmentRes;

    class RpcC2G_DecomposeEquipment : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2G_DecomposeEquipment()
        {
        }

        public override uint GetRpcType()
        {
            return 6556;
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
            Process_RpcC2G_DecomposeEquipment.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_DecomposeEquipment.OnTimeout(oArg);
        }
    }
}
