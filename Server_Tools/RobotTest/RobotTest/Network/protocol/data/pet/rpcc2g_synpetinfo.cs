using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/8/16 18:11:22

namespace XMainClient
{
	using ArgDataType = KKSG.SynPetInfoArg;
    using ResDataType = KKSG.SynPetInfoRes;

    class RpcC2G_SynPetInfo : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2G_SynPetInfo()
        {
        }

        public override uint GetRpcType()
        {
            return 6548;
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
            Process_RpcC2G_SynPetInfo.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_SynPetInfo.OnTimeout(oArg);
        }
    }
}
