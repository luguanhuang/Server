using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/6/16 14:16:19

namespace XMainClient
{
	using ArgDataType = KKSG.PetOperationOtherArg;
    using ResDataType = KKSG.PetOperationOtherRes;

    class RpcC2G_PetOperationOther : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2G_PetOperationOther()
        {
        }

        public override uint GetRpcType()
        {
            return 58525;
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
            Process_RpcC2G_PetOperationOther.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_PetOperationOther.OnTimeout(oArg);
        }
    }
}
