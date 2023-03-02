using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/11/11 21:50:29

namespace XMainClient
{
    using DataType = KKSG.GuildBuffSimpleAllInfo;

	class PtcM2C_GuildBuffSimpleInfoNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcM2C_GuildBuffSimpleInfoNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 57161;
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
			Process_PtcM2C_GuildBuffSimpleInfoNtf.Process(this);
		}

	}
}
