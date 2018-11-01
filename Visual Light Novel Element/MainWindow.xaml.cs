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
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.IO;
using Newtonsoft.Json.Linq;
using Newtonsoft.Json.Converters;

namespace Visual_Light_Novel_Element
{
    /// <summary>
    /// MainWindow.xaml에 대한 상호 작용 논리
    /// </summary>
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();
        }

        private void Type_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            if (TypeBox.SelectedItem.ToString().Contains("캐릭터"))
                Main.Content = new Character();
        }

        private void Save_Click(object sender, RoutedEventArgs e)
        {
            if (Main.Content == null)
                return;

            Microsoft.Win32.SaveFileDialog dialog = new Microsoft.Win32.SaveFileDialog();

            dialog.Filter = "Json (*.json)|*.json";
            dialog.InitialDirectory = AppDomain.CurrentDomain.BaseDirectory;

            bool? result = dialog.ShowDialog();

            if (result == true)
            {
                JObject json = ((IElement)Main.Content).GetJson();
                File.WriteAllText(dialog.FileName, json.ToString());
            }
        }
    }
}
