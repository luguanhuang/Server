using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/3/7 21:07:19

namespace XMainClient
{
    using DataType = KKSG.FriendDegreeUpNtf;

	class PtcG2C_FriendDegreeUpNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_FriendDegreeUpNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 61691;
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
			Process_PtcG2C_FriendDegreeUpNtf.Process(this);
		}

	}
}
