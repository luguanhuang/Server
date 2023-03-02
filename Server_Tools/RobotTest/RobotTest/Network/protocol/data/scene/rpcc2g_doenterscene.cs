using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/7/13 19:57:49

namespace XMainClient
{
	using ArgDataType = KKSG.DoEnterSceneArg;
    using ResDataType = KKSG.DoEnterSceneRes;

    class RpcC2G_DoEnterScene : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2G_DoEnterScene()
        {
        }

        public override uint GetRpcType()
        {
            return 45201;
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
            Process_RpcC2G_DoEnterScene.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_DoEnterScene.OnTimeout(oArg);
        }
    }
}
