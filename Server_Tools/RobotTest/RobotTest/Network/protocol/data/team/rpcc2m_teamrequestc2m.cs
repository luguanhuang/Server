using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/8/18 17:18:40

namespace XMainClient
{
	using ArgDataType = KKSG.TeamOPArg;
    using ResDataType = KKSG.TeamOPRes;

    class RpcC2M_TeamRequestC2M : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2M_TeamRequestC2M()
        {
        }

        public override uint GetRpcType()
        {
            return 30954;
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
            Process_RpcC2M_TeamRequestC2M.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2M_TeamRequestC2M.OnTimeout(oArg);
        }
    }
}
