using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Newtonsoft.Json.Linq;

namespace Visual_Light_Novel_Element
{
    interface IElement
    {
        void SetJson(JObject json);
        JObject GetJson();
    }
}
