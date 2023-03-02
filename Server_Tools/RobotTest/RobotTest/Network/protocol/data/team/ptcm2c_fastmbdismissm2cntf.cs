using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/8/19 14:29:28

namespace XMainClient
{
    using DataType = KKSG.FMDArg;

	class PtcM2C_fastMBDismissM2CNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcM2C_fastMBDismissM2CNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 38301;
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
			Process_PtcM2C_fastMBDismissM2CNtf.Process(this);
		}

	}
}
