using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/6/26 19:57:10

namespace XMainClient
{
	using ArgDataType = KKSG.GetAudioListReq;
    using ResDataType = KKSG.GetAudioListRes;

    class RpcC2A_GetAudioListReq : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2A_GetAudioListReq()
        {
        }

        public override uint GetRpcType()
        {
            return 49666;
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
            Process_RpcC2A_GetAudioListReq.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2A_GetAudioListReq.OnTimeout(oArg);
        }
    }
}
