using System;
using System.Collections.Generic;
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
using System.IO;
using MahApps.Metro.Controls;
using HM5ResourceToolCLI;

namespace HM5ResourceTool
{
    /// <summary>
    /// Interaction logic for AssemblyPathGeneratorWindow.xaml
    /// </summary>
    public partial class AssemblyPathGeneratorWindow : MetroWindow
    {
        private Dictionary<string, string> resourceIDs = new Dictionary<string, string>();

        public AssemblyPathGeneratorWindow()
        {
            InitializeComponent();
        }

        private void AssemblyPathGeneratorWindow_ContentRendered(object sender, EventArgs e)
        {
            LoadResourceIDs();
        }

        private void LoadResourceIDs()
        {
            string[] lines = File.ReadAllLines("HashMap.txt");

            foreach (string line in lines)
            {
                resourceIDs.Add(line.Split(' ')[0], line.Split(' ')[1]);
            }
        }

        private void TxtRuntimeResourceIDs_TextChanged(object sender, TextChangedEventArgs e)
        {
            List<string> lines = txtAssemblyPaths.Text.Split('\n', '\r').ToList();

            txtFilePaths.Clear();

            foreach (string line in lines)
            {
                if (line.Length > 0)
                {
                    HM5ResourceToolCLI.ZRuntimeResourceID runtimeResourceID = HM5ResourceToolCLI.ZRuntimeResourceID.Create(Convert.ToUInt64(line, 16));

                    if (runtimeResourceID.IsLibraryResource())
                    {
                        for (int i = 0; i < resourceIDs.Count; i++)
                        {
                            if (resourceIDs.ElementAt(i).Key.Contains(line.Substring(8).ToUpper()))
                            {
                                int index = runtimeResourceID.GetIndexInLibrary();

                                txtFilePaths.Text += string.Format("Index: {0}, Library: {1}\n", index, resourceIDs.ElementAt(i).Value);

                                break;
                            }
                        }
                    }
                    else
                    {
                        txtFilePaths.Text += string.Format("{0}\n", resourceIDs[line]);
                    }
                }
            }
        }
    }
}
