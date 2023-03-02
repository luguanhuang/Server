using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/9/1 16:54:12

namespace XMainClient
{
	using ArgDataType = KKSG.breakAtlas;
    using ResDataType = KKSG.breakAtlasRes;

    class RpcC2G_breakAtlas : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2G_breakAtlas()
        {
        }

        public override uint GetRpcType()
        {
            return 13728;
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
            Process_RpcC2G_breakAtlas.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_breakAtlas.OnTimeout(oArg);
        }
    }
}
