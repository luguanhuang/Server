using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/8/31 12:29:08

namespace XMainClient
{
	using ArgDataType = KKSG.EnterBattleReadySceneArg;
    using ResDataType = KKSG.EnterBattleReadySceneRes;

    class RpcC2M_EnterBattleReadyScene : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2M_EnterBattleReadyScene()
        {
        }

        public override uint GetRpcType()
        {
            return 55233;
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
            Process_RpcC2M_EnterBattleReadyScene.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2M_EnterBattleReadyScene.OnTimeout(oArg);
        }
    }
}
