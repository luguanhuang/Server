using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/9/1 16:42:53

namespace XMainClient
{
	using ArgDataType = KKSG.ActivatAtlasArg;
    using ResDataType = KKSG.ActivatAtlasRes;

    class RpcC2G_ActivatAtlas : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2G_ActivatAtlas()
        {
        }

        public override uint GetRpcType()
        {
            return 15919;
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
            Process_RpcC2G_ActivatAtlas.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_ActivatAtlas.OnTimeout(oArg);
        }
    }
}
