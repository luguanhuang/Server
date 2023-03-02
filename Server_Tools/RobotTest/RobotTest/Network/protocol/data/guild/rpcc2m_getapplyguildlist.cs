using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/11/3 19:39:17

namespace XMainClient
{
	using ArgDataType = KKSG.getapplyguildlistarg;
    using ResDataType = KKSG.getapplyguildlistres;

    class RpcC2M_getapplyguildlist : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2M_getapplyguildlist()
        {
        }

        public override uint GetRpcType()
        {
            return 31771;
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
            Process_RpcC2M_getapplyguildlist.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2M_getapplyguildlist.OnTimeout(oArg);
        }
    }
}
