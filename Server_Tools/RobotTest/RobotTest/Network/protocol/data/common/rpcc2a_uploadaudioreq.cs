using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/6/26 20:08:21

namespace XMainClient
{
	using ArgDataType = KKSG.UpLoadAudioReq;
    using ResDataType = KKSG.UpLoadAudioRes;

    class RpcC2A_UpLoadAudioReq : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2A_UpLoadAudioReq()
        {
        }

        public override uint GetRpcType()
        {
            return 3069;
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
            Process_RpcC2A_UpLoadAudioReq.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2A_UpLoadAudioReq.OnTimeout(oArg);
        }
    }
}
