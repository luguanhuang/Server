using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/5/6 16:51:28

namespace XMainClient
{
	using ArgDataType = KKSG.AbsEnterSceneArg;
    using ResDataType = KKSG.AbsEnterSceneRes;

    class RpcC2G_AbsEnterScene : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2G_AbsEnterScene()
        {
        }

        public override uint GetRpcType()
        {
            return 23463;
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
            Process_RpcC2G_AbsEnterScene.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_AbsEnterScene.OnTimeout(oArg);
        }
    }
}
