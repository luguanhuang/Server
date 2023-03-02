using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/5/3 15:18:40

namespace XMainClient
{
	using ArgDataType = KKSG.BuyTeamSceneCountP;
    using ResDataType = KKSG.BuyTeamSceneCountRet;

    class RpcC2G_BuyTeamSceneCount : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2G_BuyTeamSceneCount()
        {
        }

        public override uint GetRpcType()
        {
            return 9226;
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
            Process_RpcC2G_BuyTeamSceneCount.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_BuyTeamSceneCount.OnTimeout(oArg);
        }
    }
}
