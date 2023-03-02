using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/7/14 12:06:08

namespace XMainClient
{
	using ArgDataType = KKSG.TakeOffAllJadeArg;
    using ResDataType = KKSG.TakeOffAllJadeRes;

    class RpcC2G_TakeOffAllJade : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2G_TakeOffAllJade()
        {
        }

        public override uint GetRpcType()
        {
            return 21793;
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
            Process_RpcC2G_TakeOffAllJade.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_TakeOffAllJade.OnTimeout(oArg);
        }
    }
}
