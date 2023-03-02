using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/12/21 21:21:01

namespace XMainClient
{
	using ArgDataType = KKSG.SetHeroInHeroBattleArg;
    using ResDataType = KKSG.SetHeroInHeroBattleRes;

    class RpcC2G_SetHeroInHeroBattle : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2G_SetHeroInHeroBattle()
        {
        }

        public override uint GetRpcType()
        {
            return 18341;
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
            Process_RpcC2G_SetHeroInHeroBattle.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_SetHeroInHeroBattle.OnTimeout(oArg);
        }
    }
}
