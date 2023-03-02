using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/9/8 10:48:17

namespace XMainClient
{
	using ArgDataType = KKSG.ModifyDragonGuildNameArg;
    using ResDataType = KKSG.ModifyDragonGuildNameRes;

    class RpcC2M_ModifyDragonGuildName : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2M_ModifyDragonGuildName()
        {
        }

        public override uint GetRpcType()
        {
            return 10624;
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
            Process_RpcC2M_ModifyDragonGuildName.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2M_ModifyDragonGuildName.OnTimeout(oArg);
        }
    }
}
