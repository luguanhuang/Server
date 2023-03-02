using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/12/27 23:56:02

namespace XMainClient
{
	using ArgDataType = KKSG.BuyHeroInHeroBattleArg;
    using ResDataType = KKSG.BuyHeroInHeroBattleRes;

    class RpcC2G_BuyHeroInHeroBattle : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2G_BuyHeroInHeroBattle()
        {
        }

        public override uint GetRpcType()
        {
            return 7735;
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
            Process_RpcC2G_BuyHeroInHeroBattle.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_BuyHeroInHeroBattle.OnTimeout(oArg);
        }
    }
}
