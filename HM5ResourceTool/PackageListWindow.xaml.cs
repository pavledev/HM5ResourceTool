using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;
using System.Windows.Forms;
using Newtonsoft.Json;
using Newtonsoft.Json.Linq;
using MahApps.Metro.Controls;
using MahApps.Metro.Controls.Dialogs;
using HM5ResourceToolCLI;

namespace HM5ResourceTool
{
    /// <summary>
    /// Interaction logic for PackageListWindow.xaml
    /// </summary>
    public partial class PackageListWindow : MetroWindow
    {
        private string packageListPath;

        public PackageListWindow(string packageListPath, Settings? settings)
        {
            InitializeComponent();

            this.packageListPath = packageListPath;

            if (settings != null && settings.Theme != null)
            {
                bool hasLightTheme = settings.Theme.Split('/')[0].Equals("Light");

                if (hasLightTheme)
                {
                    treeView.ForeColor = System.Drawing.ColorTranslator.FromHtml("#000000");
                    treeView.BackColor = System.Drawing.ColorTranslator.FromHtml("#FFFFFF");
                }
                else
                {
                    treeView.ForeColor = System.Drawing.ColorTranslator.FromHtml("#FFFFFF");
                    treeView.BackColor = System.Drawing.ColorTranslator.FromHtml("#252525");
                }

                Utility.ChangeTheme(settings, this, false);
            }
        }

        private async void PackageListWindow_ContentRendered(object sender, EventArgs e)
        {
            if (!ZPackageList.ConvertPackgeListToJSON(packageListPath, out string jsonOutput))
            {
                await this.ShowMessageAsync("HM5 Resource Tool", "Failed to convert package list to JSON.");
            }

            JObject jsonObject = JObject.Parse(jsonOutput);
            SPackageListData? packageListData = JsonConvert.DeserializeObject<SPackageListData>(jsonObject.ToString());

            if (packageListData != null && packageListData.Folders != null)
            {
                int folderCount = packageListData.Folders.Count;

                for (int i = 0; i < folderCount; ++i)
                {
                    SPackageListDataFolder? packageListDataFolder = packageListData.Folders[i];
                    TreeNode folderNode = new TreeNode(packageListDataFolder.Name);

                    if (packageListDataFolder != null && packageListDataFolder.Sections != null)
                    {
                        int sectionCount = packageListDataFolder.Sections.Count;

                        for (int j = 0; j < sectionCount; ++j)
                        {
                            SPackageListDataSection? packageListDataSection = packageListDataFolder.Sections[j];

                            if (packageListDataSection != null &&
                                packageListDataSection.SceneResourceIDs != null &&
                                packageListDataSection.Resources != null)
                            {
                                int sceneResourceIDCount = packageListDataSection.SceneResourceIDs.Count;
                                int resourcesCount = packageListDataSection.Resources.Count;

                                TreeNode sectionNode = new TreeNode(j.ToString());
                                TreeNode sceneResourceIDsNode = new TreeNode("Scene Resource IDs");
                                TreeNode resourcesNode = new TreeNode("Resources");

                                for (int k = 0; k < sceneResourceIDCount; ++k)
                                {
                                    sceneResourceIDsNode.Nodes.Add(packageListDataSection.SceneResourceIDs[k]);
                                }

                                for (int k = 0; k < resourcesCount; ++k)
                                {
                                    resourcesNode.Nodes.Add(packageListDataSection.Resources[k]);
                                }

                                sectionNode.Nodes.Add(sceneResourceIDsNode);
                                sectionNode.Nodes.Add(resourcesNode);
                                folderNode.Nodes.Add(sectionNode);
                            }
                        }
                    }

                    treeView.Nodes.Add(folderNode);
                }
            }
        }
    }
}
