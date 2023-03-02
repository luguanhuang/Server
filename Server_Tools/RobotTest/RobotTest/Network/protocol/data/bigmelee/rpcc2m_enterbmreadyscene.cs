using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/6/22 8:19:11

namespace XMainClient
{
	using ArgDataType = KKSG.EnterBMReadySceneArg;
    using ResDataType = KKSG.EnterBMReadySceneRes;

    class RpcC2M_EnterBMReadyScene : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2M_EnterBMReadyScene()
        {
        }

        public override uint GetRpcType()
        {
            return 24371;
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
            Process_RpcC2M_EnterBMReadyScene.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2M_EnterBMReadyScene.OnTimeout(oArg);
        }
    }
}
