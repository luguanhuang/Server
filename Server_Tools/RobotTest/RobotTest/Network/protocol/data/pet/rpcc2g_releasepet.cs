using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/8/12 20:32:53

namespace XMainClient
{
	using ArgDataType = KKSG.ReleasePetAgr;
    using ResDataType = KKSG.ReleasePetRes;

    class RpcC2G_ReleasePet : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2G_ReleasePet()
        {
        }

        public override uint GetRpcType()
        {
            return 39612;
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
            Process_RpcC2G_ReleasePet.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_ReleasePet.OnTimeout(oArg);
        }
    }
}
