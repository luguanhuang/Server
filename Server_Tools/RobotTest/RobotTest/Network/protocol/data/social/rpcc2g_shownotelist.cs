using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2015/9/21 16:26:54

namespace XMainClient
{
	using ArgDataType = KKSG.ShowNoteListArg;
    using ResDataType = KKSG.ShowNoteListRes;

    class RpcC2G_ShowNoteList : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2G_ShowNoteList()
        {
        }

        public override uint GetRpcType()
        {
            return 65278;
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
            Process_RpcC2G_ShowNoteList.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_ShowNoteList.OnTimeout(oArg);
        }
    }
}
