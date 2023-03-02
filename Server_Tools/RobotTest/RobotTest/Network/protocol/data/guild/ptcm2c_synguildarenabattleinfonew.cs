using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/9/8 15:15:57

namespace XMainClient
{
    using DataType = KKSG.SynGuildArenaBattleInfo;

	class PtcM2C_SynGuildArenaBattleInfoNew : Protocol
	{
		public DataType Data = new DataType();

		public PtcM2C_SynGuildArenaBattleInfoNew()
		{

		}

		public override uint GetProtoType()
		{
			return 3680;
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
			Process_PtcM2C_SynGuildArenaBattleInfoNew.Process(this);
		}

	}
}
