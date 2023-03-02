using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/6/2 14:48:47

namespace XMainClient
{
    using DataType = KKSG.LoginActivityStatus;

	class PtcG2C_LoginActivityStatusNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_LoginActivityStatusNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 34113;
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
			Process_PtcG2C_LoginActivityStatusNtf.Process(this);
		}

	}
}
