using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/4/10 11:28:59

namespace XMainClient
{
    using DataType = KKSG.ModifyArg;

	class PtcM2C_ModifyGuildNameNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcM2C_ModifyGuildNameNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 18518;
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
			Process_PtcM2C_ModifyGuildNameNtf.Process(this);
		}

	}
}
