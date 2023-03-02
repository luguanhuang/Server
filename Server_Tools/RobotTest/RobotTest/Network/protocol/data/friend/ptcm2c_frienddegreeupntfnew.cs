using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/7/29 0:20:14

namespace XMainClient
{
    using DataType = KKSG.FriendDegreeUpNtf;

	class PtcM2C_FriendDegreeUpNtfNew : Protocol
	{
		public DataType Data = new DataType();

		public PtcM2C_FriendDegreeUpNtfNew()
		{

		}

		public override uint GetProtoType()
		{
			return 36126;
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
			Process_PtcM2C_FriendDegreeUpNtfNew.Process(this);
		}

	}
}
