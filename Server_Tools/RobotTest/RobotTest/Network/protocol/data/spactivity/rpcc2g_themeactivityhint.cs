using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/8/1 19:22:46

namespace XMainClient
{
	using ArgDataType = KKSG.ThemeActivityHintArg;
    using ResDataType = KKSG.ThemeActivityHintRes;

    class RpcC2G_ThemeActivityHint : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2G_ThemeActivityHint()
        {
        }

        public override uint GetRpcType()
        {
            return 39987;
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
            Process_RpcC2G_ThemeActivityHint.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_ThemeActivityHint.OnTimeout(oArg);
        }
    }
}
