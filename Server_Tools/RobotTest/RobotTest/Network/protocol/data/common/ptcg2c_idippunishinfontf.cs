using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/9/19 10:44:56

namespace XMainClient
{
    using DataType = KKSG.IdipPunishInfo;

	class PtcG2C_IdipPunishInfoNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_IdipPunishInfoNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 46304;
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
			Process_PtcG2C_IdipPunishInfoNtf.Process(this);
		}

	}
}
