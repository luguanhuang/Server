using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/7/17 17:21:11

namespace XMainClient
{
	using ArgDataType = KKSG.EnterWeddingSceneArg;
    using ResDataType = KKSG.EnterWeddingSceneRes;

    class RpcC2M_EnterWeddingScene : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2M_EnterWeddingScene()
        {
        }

        public override uint GetRpcType()
        {
            return 30316;
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
            Process_RpcC2M_EnterWeddingScene.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2M_EnterWeddingScene.OnTimeout(oArg);
        }
    }
}
