using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/9/5 16:11:14

namespace XMainClient
{
	using ArgDataType = KKSG.PlayDiceRequestArg;
    using ResDataType = KKSG.PlayDiceRequestRes;

    class RpcC2G_PlayDiceRequest : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2G_PlayDiceRequest()
        {
        }

        public override uint GetRpcType()
        {
            return 51246;
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
            Process_RpcC2G_PlayDiceRequest.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_PlayDiceRequest.OnTimeout(oArg);
        }
    }
}
