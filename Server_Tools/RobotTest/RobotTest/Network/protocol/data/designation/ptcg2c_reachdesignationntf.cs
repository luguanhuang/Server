using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/6/2 15:51:57

namespace XMainClient
{
    using DataType = KKSG.ReachDesignationNtf;

	class PtcG2C_ReachDesignationNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_ReachDesignationNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 17457;
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
			Process_PtcG2C_ReachDesignationNtf.Process(this);
		}

	}
}
