using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/9/8 16:40:23

namespace XMainClient
{
	using ArgDataType = KKSG.FetchDragonGuildListArg;
    using ResDataType = KKSG.FetchDragonGuildRes;

    class RpcC2M_FetchDragonGuildList : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2M_FetchDragonGuildList()
        {
        }

        public override uint GetRpcType()
        {
            return 23518;
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
            Process_RpcC2M_FetchDragonGuildList.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2M_FetchDragonGuildList.OnTimeout(oArg);
        }
    }
}
