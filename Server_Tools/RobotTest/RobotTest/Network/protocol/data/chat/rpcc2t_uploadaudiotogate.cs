using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/8/11 16:53:22

namespace XMainClient
{
	using ArgDataType = KKSG.UpLoadAudioReq;
    using ResDataType = KKSG.UpLoadAudioRes;

    class RpcC2T_UpLoadAudioToGate : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2T_UpLoadAudioToGate()
        {
        }

        public override uint GetRpcType()
        {
            return 23176;
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
            Process_RpcC2T_UpLoadAudioToGate.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2T_UpLoadAudioToGate.OnTimeout(oArg);
        }
    }
}
