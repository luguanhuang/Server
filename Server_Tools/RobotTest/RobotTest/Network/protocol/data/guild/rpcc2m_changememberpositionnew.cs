using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/9/20 17:44:57

namespace XMainClient
{
	using ArgDataType = KKSG.ChangeGuildPositionArg;
    using ResDataType = KKSG.ChangeGuildPositionRes;

    class RpcC2M_ChangeMemberPositionNew : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2M_ChangeMemberPositionNew()
        {
        }

        public override uint GetRpcType()
        {
            return 13625;
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
            Process_RpcC2M_ChangeMemberPositionNew.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2M_ChangeMemberPositionNew.OnTimeout(oArg);
        }
    }
}
