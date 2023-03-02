using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/10/13 18:27:15

namespace XMainClient
{
    using DataType = KKSG.GuildCastFeats;

	class PtcM2C_GuildCastFeatsNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcM2C_GuildCastFeatsNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 32885;
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
			Process_PtcM2C_GuildCastFeatsNtf.Process(this);
		}

	}
}
