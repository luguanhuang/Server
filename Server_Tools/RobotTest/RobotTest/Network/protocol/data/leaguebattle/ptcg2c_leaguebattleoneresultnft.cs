using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/1/9 7:58:25

namespace XMainClient
{
    using DataType = KKSG.LeagueBattleOneResultNtf;

	class PtcG2C_LeagueBattleOneResultNft : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_LeagueBattleOneResultNft()
		{

		}

		public override uint GetProtoType()
		{
			return 40599;
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
			Process_PtcG2C_LeagueBattleOneResultNft.Process(this);
		}

	}
}
