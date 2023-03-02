using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/12/27 20:22:52

namespace XMainClient
{
    //using DataType = KKSG.;

	class PtcG2C_CloseChooseHeroNtf : Protocol
	{
		//public DataType Data = new DataType();

		public PtcG2C_CloseChooseHeroNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 38670;
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
			Process_PtcG2C_CloseChooseHeroNtf.Process(this);
		}

	}
}
