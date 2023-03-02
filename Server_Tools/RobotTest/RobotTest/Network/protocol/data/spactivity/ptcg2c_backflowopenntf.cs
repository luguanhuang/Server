using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/10/21 16:38:38

namespace XMainClient
{
    using DataType = KKSG.BackFlowOpenNtf;

	class PtcG2C_BackFlowOpenNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_BackFlowOpenNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 27749;
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
			Process_PtcG2C_BackFlowOpenNtf.Process(this);
		}

	}
}
