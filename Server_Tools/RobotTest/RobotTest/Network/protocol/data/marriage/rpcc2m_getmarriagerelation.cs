using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/7/24 20:59:09

namespace XMainClient
{
	using ArgDataType = KKSG.GetMarriageRelationArg;
    using ResDataType = KKSG.GetMarriageRelationRes;

    class RpcC2M_GetMarriageRelation : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2M_GetMarriageRelation()
        {
        }

        public override uint GetRpcType()
        {
            return 13460;
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
            Process_RpcC2M_GetMarriageRelation.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2M_GetMarriageRelation.OnTimeout(oArg);
        }
    }
}
