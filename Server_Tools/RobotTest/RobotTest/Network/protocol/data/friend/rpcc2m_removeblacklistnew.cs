using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/7/28 23:49:44

namespace XMainClient
{
	using ArgDataType = KKSG.RemoveBlackListArg;
    using ResDataType = KKSG.RemoveBlackListRes;

    class RpcC2M_RemoveBlackListNew : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2M_RemoveBlackListNew()
        {
        }

        public override uint GetRpcType()
        {
            return 38702;
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
            Process_RpcC2M_RemoveBlackListNew.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2M_RemoveBlackListNew.OnTimeout(oArg);
        }
    }
}
