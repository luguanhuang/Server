using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/6/26 19:46:24

namespace XMainClient
{
	using ArgDataType = KKSG.GetAudioReq;
    using ResDataType = KKSG.GetAudioRes;

    class RpcC2A_GetAudioReq : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2A_GetAudioReq()
        {
        }

        public override uint GetRpcType()
        {
            return 5073;
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
            Process_RpcC2A_GetAudioReq.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2A_GetAudioReq.OnTimeout(oArg);
        }
    }
}
