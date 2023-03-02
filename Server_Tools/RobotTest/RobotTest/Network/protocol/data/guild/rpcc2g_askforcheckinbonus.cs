using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/6/16 14:25:48

namespace XMainClient
{
	using ArgDataType = KKSG.AskForCheckInBonusArg;
    using ResDataType = KKSG.AskForCheckInBonusRes;

    class RpcC2G_AskForCheckInBonus : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2G_AskForCheckInBonus()
        {
        }

        public override uint GetRpcType()
        {
            return 32843;
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
            Process_RpcC2G_AskForCheckInBonus.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_AskForCheckInBonus.OnTimeout(oArg);
        }
    }
}
