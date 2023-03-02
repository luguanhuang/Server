using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/6/26 20:59:19

namespace XMainClient
{
	using ArgDataType = KKSG.BuySpriteEggArg;
    using ResDataType = KKSG.BuySpriteEggRes;

    class RpcC2G_BuySpriteEgg : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2G_BuySpriteEgg()
        {
        }

        public override uint GetRpcType()
        {
            return 34552;
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
            Process_RpcC2G_BuySpriteEgg.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_BuySpriteEgg.OnTimeout(oArg);
        }
    }
}
