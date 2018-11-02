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
        public enum ElementType
        {
            Character,
            Background,
            Stuff
        }

        public MainWindow()
        {
            InitializeComponent();
            TypeBox.Items.Add(ElementType.Character);
            TypeBox.Items.Add(ElementType.Background);
            TypeBox.Items.Add(ElementType.Stuff);
        }

        private void Type_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            ElementType sel = (ElementType)TypeBox.SelectedItem;
            if (sel == ElementType.Character)
                Main.Content = new Character();
            if (sel == ElementType.Background)
                Main.Content = new Background();
            if (sel == ElementType.Stuff)
                Main.Content = new Stuff();
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

        private void Load_Click(object sender, RoutedEventArgs e)
        {
            Microsoft.Win32.OpenFileDialog dialog = new Microsoft.Win32.OpenFileDialog();

            dialog.Filter = "Json (*.json)|*.json";
            dialog.InitialDirectory = AppDomain.CurrentDomain.BaseDirectory;

            bool? result = dialog.ShowDialog();
            
            if (result == true)
            {
                var content = File.ReadAllText(dialog.FileName);
                JObject json = JObject.Parse(content);

                string type = (string)json["TYPE"];

                IElement element = null;

                if (type == "CHARACTER")
                {
                    element = new Character();
                    TypeBox.SelectedItem = ElementType.Character;
                }
                if (type == "BACKGROUND")
                {
                    element = new Background();
                    TypeBox.SelectedItem = ElementType.Background;
                }
                if (type == "STUFF")
                {
                    element = new Stuff();
                    TypeBox.SelectedItem = ElementType.Stuff;
                }

                element.SetJson(json);
                Main.Content = element;
            }
        }
    }
}
