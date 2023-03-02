using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.ComponentModel;

namespace DataWatcher
{
    class CommonConverter : ExpandableObjectConverter
    {
        public override object ConvertTo(ITypeDescriptorContext context, System.Globalization.CultureInfo culture, object value, Type destType)
        {
            if (destType == typeof(string))
            {
                if (value is KKSG.BagContent)
                {
                    KKSG.BagContent bag = (KKSG.BagContent)value;
                    return "Equip " + bag.Equips.Count + ", Fashion " + bag.Fashions.Count + ", Emblems " + bag.Emblems.Count + ", Item " + bag.Items.Count;
                }

                if (value is KKSG.RoleBrief )
   			    {
                    KKSG.RoleBrief rb = (KKSG.RoleBrief)value;
   				    return rb.name + ", " + rb.accountID;
   			    }

                return destType;
            }

            return base.ConvertTo(context, culture, value, destType);
        }
    }
}
