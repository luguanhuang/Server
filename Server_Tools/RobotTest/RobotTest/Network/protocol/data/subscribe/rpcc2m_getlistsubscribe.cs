using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/1/10 14:37:54

namespace XMainClient
{
	using ArgDataType = KKSG.GetListSubscribeArg;
    using ResDataType = KKSG.GetListSubscribeRes;

    class RpcC2M_GetListSubscribe : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2M_GetListSubscribe()
        {
        }

        public override uint GetRpcType()
        {
            return 1403;
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
            Process_RpcC2M_GetListSubscribe.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2M_GetListSubscribe.OnTimeout(oArg);
        }
    }
}
