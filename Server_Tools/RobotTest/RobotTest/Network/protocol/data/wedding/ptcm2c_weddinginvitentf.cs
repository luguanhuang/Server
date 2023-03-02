using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/7/25 9:58:12

namespace XMainClient
{
    using DataType = KKSG.WeddingInviteNtf;

	class PtcM2C_WeddingInviteNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcM2C_WeddingInviteNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 35104;
		}

		public override void Serialize(MemoryStream stream)
		{
			Serializer.Serialize(stream, Data);
		}

		public override void DeSerialize(MemoryStream stream)
		{
			Data = Serializer.Deserialize<DataType>(stream);
		}

		public override void Process()
		{
			Process_PtcM2C_WeddingInviteNtf.Process(this);
		}

	}
}
