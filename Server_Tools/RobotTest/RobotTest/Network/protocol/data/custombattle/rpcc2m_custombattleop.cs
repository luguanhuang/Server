using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/4/7 21:30:43

namespace XMainClient
{
	using ArgDataType = KKSG.CustomBattleOpArg;
    using ResDataType = KKSG.CustomBattleOpRes;

    class RpcC2M_CustomBattleOp : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2M_CustomBattleOp()
        {
        }

        public override uint GetRpcType()
        {
            return 12314;
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
            Process_RpcC2M_CustomBattleOp.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2M_CustomBattleOp.OnTimeout(oArg);
        }
    }
}
