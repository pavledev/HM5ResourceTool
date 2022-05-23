using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace HM5ResourceTool
{
    internal class SPackageListData
    {
        public SPackageListData()
        {
            Folders = null;
        }

        public List<SPackageListDataFolder>? Folders;
    }

    internal class SPackageListDataFolder
    {
        public SPackageListDataFolder()
        {
            Name = null;
            Sections = null;
        }

        public string? Name;
        public List<SPackageListDataSection>? Sections;
    }

    internal class SPackageListDataSection
    {
        public SPackageListDataSection()
        {
            SceneResourceIDs = null;
            Resources = null;
        }

        public List<string>? SceneResourceIDs;
        public List<string>? Resources;
    }
}
