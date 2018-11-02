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

namespace Visual_Light_Novel_Element
{
    /// <summary>
    /// Character.xaml에 대한 상호 작용 논리
    /// </summary>

    public enum CharacterPart
    {
        HEAD,
        LEFT_BODY,
        RIGHT_BODY,
        FULL_BODY,
        FULL
    }

    public partial class Character : Page, IElement
    {
        public Character()
        {
            InitializeComponent();
        }

        public JObject GetJson()
        {
            var json = new JObject();

            json.Add("TYPE", "CHARACTER");
            json.Add("NAME", Name.Text);
            json.Add("EXPLAIN", new TextRange(Explain.Document.ContentStart, Explain.Document.ContentEnd).Text);

            var state = new JArray();
            foreach (Tuple<Tuple<string, CharacterPart>, string> item in StateList.Items)
            {
                var list = new JObject();
                list.Add("NAME", item.Item1.Item1);
                list.Add("PART", item.Item1.Item2.ToString());
                list.Add("PATH", item.Item2);
                state.Add(list);
            }

            json.Add("STATE", state);

            return json;
        }

        public void SetJson(JObject json)
        {
            Name.Text = (string)json["NAME"];
            Explain.Document.Blocks.Clear();
            Explain.Document.Blocks.Add(new Paragraph(new Run((string)json["EXPLAIN"])));

            foreach (var item in json["STATE"])
            {
                string state = (string)item["NAME"];
                string partName = (string)item["PART"];
                string path = (string)item["PATH"];
                CharacterPart part = CharacterPart.HEAD;
                if (partName == "HEAD") part = CharacterPart.HEAD;
                if (partName == "LEFT_BODY") part = CharacterPart.LEFT_BODY;
                if (partName == "RIGHT_BODY") part = CharacterPart.RIGHT_BODY;
                if (partName == "FULL_BODY") part = CharacterPart.FULL_BODY;
                if (partName == "FULL") part = CharacterPart.FULL;
                Tuple<Tuple<string, CharacterPart>, string> data = new Tuple<Tuple<string, CharacterPart>, string>(new Tuple<string, CharacterPart>(state, part), path);
                StateList.Items.Add(data);
            }
        }

        private void Add_Click(object sender, RoutedEventArgs e)
        {
            Microsoft.Win32.OpenFileDialog dialog = new Microsoft.Win32.OpenFileDialog();

            dialog.Filter = "Image (*.png, *.jpg)|*.png;*.jpg";
            dialog.InitialDirectory = AppDomain.CurrentDomain.BaseDirectory;

            bool? result = dialog.ShowDialog();

            if (result == true)
            {
                string fileName = dialog.FileName;
                fileName = fileName.Remove(0, dialog.InitialDirectory.Length);
                CharacterDialogue input = new CharacterDialogue();
                if (input.ShowDialog() == true)
                {
                    string stateName = input.Text;
                    CharacterPart part = input.Part;
                    Tuple<Tuple<string, CharacterPart>, string> data = new Tuple<Tuple<string, CharacterPart>, string>(new Tuple<string, CharacterPart>(stateName, part), fileName);
                    StateList.Items.Add(data);
                }
            }
        }

        private void Delete_Click(object sender, RoutedEventArgs e)
        {
            StateList.Items.Remove(StateList.SelectedItem);
        }

        private void StateList_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            Preview_HEAD.Source = null;
            Preview_LEFT_BODY.Source = null;
            Preview_RIGHT_BODY.Source = null;
            Preview_FULL_BODY.Source = null;
            Preview_FULL.Source = null;

            if (StateList.SelectedItem == null)
                return;

            Tuple<Tuple<string, CharacterPart>, string> select = (Tuple<Tuple<string, CharacterPart>, string>)StateList.SelectedItem;

            foreach (Tuple<Tuple<string, CharacterPart>, string> item in StateList.Items)
            {
                if (select.Item1.Item1 == item.Item1.Item1)
                {
                    string path = AppDomain.CurrentDomain.BaseDirectory + item.Item2;
                    BitmapImage image = new BitmapImage();
                    image.BeginInit();
                    image.UriSource = new Uri(path);
                    image.EndInit();
                    if (item.Item1.Item2 == CharacterPart.HEAD) Preview_HEAD.Source = image;
                    if (item.Item1.Item2 == CharacterPart.LEFT_BODY) Preview_LEFT_BODY.Source = image;
                    if (item.Item1.Item2 == CharacterPart.RIGHT_BODY) Preview_RIGHT_BODY.Source = image;
                    if (item.Item1.Item2 == CharacterPart.FULL_BODY) Preview_FULL_BODY.Source = image;
                    if (item.Item1.Item2 == CharacterPart.FULL) Preview_FULL.Source = image;
                }
            }
        }
    }
}
