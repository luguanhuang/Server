using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/10/16 11:31:07

namespace XMainClient
{
	using ArgDataType = KKSG.ChooseSpecialEffectsArg;
    using ResDataType = KKSG.ChooseSpecialEffectsRes;

    class RpcC2G_ChooseSpecialEffects : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2G_ChooseSpecialEffects()
        {
        }

        public override uint GetRpcType()
        {
            return 55040;
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
            Process_RpcC2G_ChooseSpecialEffects.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_ChooseSpecialEffects.OnTimeout(oArg);
        }
    }
}
