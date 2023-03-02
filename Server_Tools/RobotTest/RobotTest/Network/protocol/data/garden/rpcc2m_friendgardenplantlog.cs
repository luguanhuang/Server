using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/10/9 11:03:11

namespace XMainClient
{
	using ArgDataType = KKSG.FriendGardenPlantLogArg;
    using ResDataType = KKSG.FriendGardenPlantLogRes;

    class RpcC2M_FriendGardenPlantLog : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2M_FriendGardenPlantLog()
        {
        }

        public override uint GetRpcType()
        {
            return 33646;
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
            Process_RpcC2M_FriendGardenPlantLog.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2M_FriendGardenPlantLog.OnTimeout(oArg);
        }
    }
}
