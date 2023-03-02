using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2015/9/21 16:04:04

namespace XMainClient
{
	using ArgDataType = KKSG.RemoveNoteArg;
    using ResDataType = KKSG.RemoveNoteRes;

    class RpcC2G_RemoveNote : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2G_RemoveNote()
        {
        }

        public override uint GetRpcType()
        {
            return 33231;
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
            Process_RpcC2G_RemoveNote.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_RemoveNote.OnTimeout(oArg);
        }
    }
}
