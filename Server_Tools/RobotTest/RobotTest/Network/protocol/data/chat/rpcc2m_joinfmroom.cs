using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/11/23 17:44:59

namespace XMainClient
{
	using ArgDataType = KKSG.JoinLargeRoomArg;
    using ResDataType = KKSG.JoinLargeRoomRes;

    class RpcC2M_JoinFmRoom : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2M_JoinFmRoom()
        {
        }

        public override uint GetRpcType()
        {
            return 25303;
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
            Process_RpcC2M_JoinFmRoom.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2M_JoinFmRoom.OnTimeout(oArg);
        }
    }
}
