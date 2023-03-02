using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/7/28 23:49:14

namespace XMainClient
{
	using ArgDataType = KKSG.AddBlackListArg;
    using ResDataType = KKSG.AddBlackListRes;

    class RpcC2M_AddBlackListNew : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2M_AddBlackListNew()
        {
        }

        public override uint GetRpcType()
        {
            return 265;
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
            Process_RpcC2M_AddBlackListNew.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2M_AddBlackListNew.OnTimeout(oArg);
        }
    }
}
