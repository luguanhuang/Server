using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/9/2 17:37:24

namespace XMainClient
{
	using ArgDataType = KKSG.SpriteOperationArg;
    using ResDataType = KKSG.SpriteOperationRes;

    class RpcC2G_SpriteOperation : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2G_SpriteOperation()
        {
        }

        public override uint GetRpcType()
        {
            return 62961;
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
            Process_RpcC2G_SpriteOperation.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_SpriteOperation.OnTimeout(oArg);
        }
    }
}
