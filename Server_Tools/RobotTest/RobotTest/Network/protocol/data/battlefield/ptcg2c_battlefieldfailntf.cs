using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/9/4 16:12:51

namespace XMainClient
{
    //using DataType = KKSG.;

	class PtcG2C_BattleFieldFailNtf : Protocol
	{
		//public DataType Data = new DataType();

		public PtcG2C_BattleFieldFailNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 56799;
		}

		public override void Serialize(MemoryStream stream)
		{
			//Serializer.Serialize(stream, Data);
		}

		public override void DeSerialize(MemoryStream stream)
		{
			//Data = Serializer.Deserialize<DataType>(stream);
		}

		public override void Process()
		{
			Process_PtcG2C_BattleFieldFailNtf.Process(this);
		}

	}
}
