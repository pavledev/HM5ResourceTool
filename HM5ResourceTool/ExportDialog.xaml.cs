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
using MahApps.Metro.Controls;

namespace HM5ResourceTool
{
    /// <summary>
    /// Interaction logic for ExportDialog.xaml
    /// </summary>
    public partial class ExportDialog : MetroWindow
    {
        private List<string> extensions;

        public ExportDialog(List<string> extensions)
        {
            InitializeComponent();

            int count = extensions.Count;

            for (int i = 0; i < count; ++i)
            {
                RadioButton radioButton = new RadioButton();

                radioButton.Content = extensions[i];
                radioButton.Checked += RadioButton_Checked;

                extensionsGrid.Children.Add(radioButton);
            }

            this.extensions = extensions;
        }

        public uint Option
        {
            get;
            set;
        }

        private void RadioButton_Checked(object sender, RoutedEventArgs e)
        {
            RadioButton? radioButton = sender as RadioButton;
            int count = extensions.Count;

            if (radioButton != null)
            {
                for (int i = 0; i < count; ++i)
                {
                    if (radioButton.Content.ToString() == extensions[i])
                    {
                        Option = (uint)i;

                        break;
                    }
                }
            }
        }
    }
}
