using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/5/5 16:57:06

namespace XMainClient
{
    using DataType = KKSG.guildcamppartyNotifyNtf;

	class PtcG2C_guildcamppartyNotify : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_guildcamppartyNotify()
		{

		}

		public override uint GetProtoType()
		{
			return 23338;
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
			Process_PtcG2C_guildcamppartyNotify.Process(this);
		}

	}
}
