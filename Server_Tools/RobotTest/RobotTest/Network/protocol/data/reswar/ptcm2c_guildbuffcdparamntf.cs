using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/11/30 14:56:03

namespace XMainClient
{
    using DataType = KKSG.GuildBuffCDParam;

	class PtcM2C_GuildBuffCDParamNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcM2C_GuildBuffCDParamNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 4703;
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
			Process_PtcM2C_GuildBuffCDParamNtf.Process(this);
		}

	}
}
