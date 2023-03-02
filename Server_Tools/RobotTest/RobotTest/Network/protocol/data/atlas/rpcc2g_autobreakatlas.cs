using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/9/1 16:52:57

namespace XMainClient
{
	using ArgDataType = KKSG.AutoBreakAtlasArg;
    using ResDataType = KKSG.AutoBreakAtlasRes;

    class RpcC2G_AutoBreakAtlas : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2G_AutoBreakAtlas()
        {
        }

        public override uint GetRpcType()
        {
            return 23263;
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
            Process_RpcC2G_AutoBreakAtlas.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_AutoBreakAtlas.OnTimeout(oArg);
        }
    }
}
