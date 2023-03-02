using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/10/10 18:47:33

namespace XMainClient
{
	using ArgDataType = KKSG.ChangeNameArg;
    using ResDataType = KKSG.ChangeNameRes;

    class RpcC2G_ChangeName : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2G_ChangeName()
        {
        }

        public override uint GetRpcType()
        {
            return 7251;
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
            Process_RpcC2G_ChangeName.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_ChangeName.OnTimeout(oArg);
        }
    }
}
