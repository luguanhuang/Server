using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/9/30 15:03:56

namespace XMainClient
{
    using DataType = KKSG.HallIconPara;

	class PtcM2C_HallIconMsNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcM2C_HallIconMsNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 49540;
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
			Process_PtcM2C_HallIconMsNtf.Process(this);
		}

	}
}
