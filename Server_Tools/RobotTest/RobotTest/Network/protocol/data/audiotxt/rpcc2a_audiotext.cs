using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/2/6 19:57:21

namespace XMainClient
{
	using ArgDataType = KKSG.AudioTextArg;
    using ResDataType = KKSG.AudioTextRes;

    class RpcC2A_AudioText : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2A_AudioText()
        {
        }

        public override uint GetRpcType()
        {
            return 42254;
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
            Process_RpcC2A_AudioText.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2A_AudioText.OnTimeout(oArg);
        }
    }
}
