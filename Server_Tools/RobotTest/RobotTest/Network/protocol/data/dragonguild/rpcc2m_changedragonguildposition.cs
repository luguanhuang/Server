using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/9/13 19:59:43

namespace XMainClient
{
	using ArgDataType = KKSG.ChangeDragonGuildPositionArg;
    using ResDataType = KKSG.ChangeDragonGuildPositionRes;

    class RpcC2M_ChangeDragonGuildPosition : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2M_ChangeDragonGuildPosition()
        {
        }

        public override uint GetRpcType()
        {
            return 3888;
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
            Process_RpcC2M_ChangeDragonGuildPosition.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2M_ChangeDragonGuildPosition.OnTimeout(oArg);
        }
    }
}
