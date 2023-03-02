using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/9/8 11:45:45

namespace XMainClient
{
    using DataType = KKSG.DragonGuildNameNtf;

	class PtcM2C_DragonGuildNameNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcM2C_DragonGuildNameNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 35553;
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
			Process_PtcM2C_DragonGuildNameNtf.Process(this);
		}

	}
}
