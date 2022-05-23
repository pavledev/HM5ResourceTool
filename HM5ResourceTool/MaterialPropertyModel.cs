using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace HM5ResourceTool
{
    internal class MaterialPropertyModel
    {
        public MaterialPropertyModel()
        {
            MaterialProperties = new ObservableCollection<MaterialProperty>();
        }

        public ObservableCollection<MaterialProperty>? MaterialProperties { get; }
    }
}
