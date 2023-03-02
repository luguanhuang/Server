using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/10/17 11:08:28

namespace XMainClient
{
	using ArgDataType = KKSG.EnterLeisureSceneArg;
    using ResDataType = KKSG.EnterLeisureSceneRes;

    class RpcC2G_EnterLeisureScene : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2G_EnterLeisureScene()
        {
        }

        public override uint GetRpcType()
        {
            return 62361;
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
            Process_RpcC2G_EnterLeisureScene.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_EnterLeisureScene.OnTimeout(oArg);
        }
    }
}
