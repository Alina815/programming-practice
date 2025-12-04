using System;
using System.Collections.Generic;
using System.Globalization;
using System.IO;
using System.Windows.Forms;
using lab_5;

namespace lab_5
{
    public partial class MyForm : Form
    {
        private readonly ATE ate = new ATE();
        private readonly List<string>  tariff_list = new List<string>();
        private readonly List<string> history = new List<string>();
        private TextBox txtPrice;
        private TextBox txtDiscount;
        private TextBox txtCity1;
        private TextBox txtCity2;
        private Label lblPrice;
        private Label lblDiscount;
        private Label lblCity1;
        private Label lblCity2;
        private Button btnAddUsual;
        private Button btnAddDiscount;
        private Button btnCalculateAverage;
        private RichTextBox txtResult;
        private Button btnShowList;
        private Button btnShowHistory;

        public MyForm()
        {
            InitializeComponent();
            string dbPath = Path.Combine(Application.StartupPath, "tariffs.db");
            if (ate.InitializeDatabase(dbPath))
            {
                ate.LoadFromDatabase();
                MessageBox.Show("Подключено к базе данных.", "ИНФО", MessageBoxButtons.OK, MessageBoxIcon.Information);
                tariff_list = ate.GetLists();
            } else
            {
                MessageBox.Show("База данных недоступна. Работа в локальном режиме.", "ПРЕДУПРЕЖДЕНИЕ", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                LoadListFromFile("tariffs.txt");
            }
            LoadHistoryFromFile("history.txt");
        }

        override protected void OnFormClosing(FormClosingEventArgs e)
        {
            SaveHistoryToFile("history.txt");
            if (ate.dbMode)
                ate.SaveToDatabase();
            else
                SaveListToFile("tariffs.txt");
            base.OnFormClosing(e);
        }

        private void InitializeComponent()
        {
            txtPrice = new TextBox();
            txtDiscount = new TextBox();
            txtCity1 = new TextBox();
            txtCity2 = new TextBox();
            lblPrice = new Label();
            lblDiscount = new Label();
            lblCity1 = new Label();
            lblCity2 = new Label();
            btnAddUsual = new Button();
            btnAddDiscount = new Button();
            btnCalculateAverage = new Button();
            txtResult = new RichTextBox();
            btnShowList = new Button();
            btnShowHistory = new Button();
            // 
            // txtPrice
            // 
            txtPrice.Location = new Point(180, 20);
            txtPrice.Name = "txtPrice";
            txtPrice.Size = new Size(100, 22);
            txtPrice.TabIndex = 0;
            // 
            // txtDiscount
            // 
            txtDiscount.Location = new Point(180, 50);
            txtDiscount.Name = "txtDiscount";
            txtDiscount.Size = new Size(100, 22);
            txtDiscount.TabIndex = 1;
            // 
            // txtCity1
            // 
            txtCity1.Location = new Point(180, 80);
            txtCity1.Name = "txtCity1";
            txtCity1.Size = new Size(100, 22);
            txtCity1.TabIndex = 2;
            // 
            // txtCity2
            // 
            txtCity2.Location = new Point(180, 110);
            txtCity2.Name = "txtCity2";
            txtCity2.Size = new Size(100, 22);
            txtCity2.TabIndex = 3;
            // 
            // lblPrice
            // 
            lblPrice.Location = new Point(20, 20);
            lblPrice.Name = "lblPrice";
            lblPrice.Size = new Size(100, 20);
            lblPrice.Text = "Цена:";
            // 
            // lblDiscount
            // 
            lblDiscount.Location = new Point(20, 50);
            lblDiscount.Name = "lblDiscount";
            lblDiscount.Size = new Size(100, 20);
            lblDiscount.Text = "Скидка (%):";
            // 
            // lblCity1
            // 
            lblCity1.Location = new Point(20, 80);
            lblCity1.Name = "lblCity1";
            lblCity1.Size = new Size(100, 20);
            lblCity1.Text = "Город 1:";
            // 
            // lblCity2
            // 
            lblCity2.Location = new Point(20, 110);
            lblCity2.Name = "lblCity2";
            lblCity2.Size = new Size(100, 20);
            lblCity2.Text = "Город 2:";
            // 
            // btnAddUsual
            // 
            btnAddUsual.Location = new Point(20, 230);
            btnAddUsual.Name = "btnAddUsual";
            btnAddUsual.Size = new Size(260, 25);
            btnAddUsual.Text = "Добавить обычный тариф";
            btnAddUsual.TabIndex = 4;
            btnAddUsual.Click += btnAddUsual_Click;
            // 
            // btnAddDiscount
            // 
            btnAddDiscount.Location = new Point(20, 270);
            btnAddDiscount.Name = "btnAddDiscount";
            btnAddDiscount.Size = new Size(260, 25);
            btnAddDiscount.Text = "Добавить льготный тариф";
            btnAddDiscount.TabIndex = 5;
            btnAddDiscount.Click += btnAddDiscount_Click;
            // 
            // btnCalculateAverage
            // 
            btnCalculateAverage.Location = new Point(20, 310);
            btnCalculateAverage.Name = "btnCalculateAverage";
            btnCalculateAverage.Size = new Size(260, 25);
            btnCalculateAverage.Text = "Вычислить среднюю цену";
            btnCalculateAverage.TabIndex = 6;
            btnCalculateAverage.Click += btnCalculateAverage_Click;
            // 
            // txtResult
            // 
            txtResult.Location = new Point(375, 20);
            txtResult.Name = "txtResult";
            txtResult.Size = new Size(215, 315);
            txtResult.ReadOnly = true;
            txtResult.Text = "";
            // 
            // btnShowList
            // 
            btnShowList.Location = new Point(375, 370);
            btnShowList.Name = "btnShowList";
            btnShowList.Size = new Size(215, 30);
            btnShowList.Text = "Показать список тарифов";
            btnShowList.TabIndex = 7;
            btnShowList.Click += btnShowList_Click;
            // 
            // btnShowHistory
            // 
            btnShowHistory.Location = new Point(375, 420);
            btnShowHistory.Name = "btnShowHistory";
            btnShowHistory.Size = new Size(215, 30);
            btnShowHistory.Text = "Показать историю операций";
            btnShowHistory.TabIndex = 8;
            btnShowHistory.Click += btnShowHistory_Click;
            // 
            // MyForm
            // 
            ClientSize = new Size(610, 470);
            Controls.Add(txtPrice);
            Controls.Add(txtDiscount);
            Controls.Add(txtCity1);
            Controls.Add(txtCity2);
            Controls.Add(lblPrice);
            Controls.Add(lblDiscount);
            Controls.Add(lblCity1);
            Controls.Add(lblCity2);
            Controls.Add(btnAddUsual);
            Controls.Add(btnAddDiscount);
            Controls.Add(btnCalculateAverage);
            Controls.Add(txtResult);
            Controls.Add(btnShowList);
            Controls.Add(btnShowHistory);
            Name = "MyForm";
            Text = "Система управления тарифами";
        }

        void btnAddUsual_Click(object sender, EventArgs e)
        {
            if (!CheckPrice(txtPrice, "Цена") ||
                !CheckCity(txtCity1, "Город 1") ||
                !CheckCity(txtCity2, "Город 2"))
                return;
            double price = double.Parse(txtPrice.Text, CultureInfo.InvariantCulture);
            string city1 = txtCity1.Text;
            string city2 = txtCity2.Text;
            try
            {
                ate.AddUsualTariff(price, city1, city2);
                tariff_list.Add($"u;{city1};{city2};{price:F2}");
                history.Add($"Добавлен обычный тариф: {city1} -> {city2} = {price:F2}.");
                MessageBox.Show("Обычный тариф добавлен!", "УСПЕХ", MessageBoxButtons.OK, MessageBoxIcon.Information);
                ClearInputFields();
            }
            catch (TariffException ex)
            {
                MessageBox.Show(ex.Message, "ОШИБКА", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }
        private void btnAddDiscount_Click(object sender, EventArgs e)
        {
            if (!CheckPrice(txtPrice, "Цена") ||
                !CheckDiscount(txtDiscount, "Скидка (%)") ||
                !CheckCity(txtCity1, "Город 1") ||
                !CheckCity(txtCity2, "Город 2"))
                return;
            double price = double.Parse(txtPrice.Text, CultureInfo.InvariantCulture);
            double discount = double.Parse(txtDiscount.Text, CultureInfo.InvariantCulture);
            string city1 = txtCity1.Text;
            string city2 = txtCity2.Text;
            try
            {
                ate.AddDiscountTariff(price, discount, city1, city2);
                tariff_list.Add($"d;{city1};{city2};{price:F2};{discount:F2}");
                history.Add($"Добавлен льготный тариф: {city1} -> {city2} = {price:F2} % {discount:F2}.");
                MessageBox.Show("Льготный тариф добавлен!", "УСПЕХ", MessageBoxButtons.OK, MessageBoxIcon.Information);
                ClearInputFields();
            }
            catch (TariffException ex)
            {
                MessageBox.Show(ex.Message, "ОШИБКА", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }
        private void btnCalculateAverage_Click(object sender, EventArgs e)
        {
            try
            {
                double average = ate.CalculateAveragePrice();
                txtResult.Text = $"средняя цена тарифов = {average:F2}";
                history.Add($"Вычислена средняя цена тарифов = {average:F2}.");
            }
            catch (TariffException ex)
            {
                MessageBox.Show(ex.Message, "ОШИБКА", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }
        private void btnShowList_Click(object sender, EventArgs e)
        {
            var list_form = new Values(ate, tariff_list);
            list_form.ShowDialog();
        }
        private void btnShowHistory_Click(object sender, EventArgs e)
        {
            var history_form = new Values(history);
            history_form.ShowDialog();
        }

        private bool CheckPrice(TextBox tb, string name)
        {
            if (string.IsNullOrEmpty(tb.Text))
            {
                MessageBox.Show($"Поле '{name}' не может быть пустым.", "ОШИБКА", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return false;
            }
            if (!double.TryParse(tb.Text, NumberStyles.Float, CultureInfo.InvariantCulture, out double value) ||
                    double.IsInfinity(value) || double.IsNaN(value) || value < 0)
            {
                MessageBox.Show($"В поле '{name}' должно быть введено неотрицательное вещественное значение.", "ОШИБКА", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return false;
            }
            return true;
        }
        private bool CheckDiscount(TextBox tb, string name)
        {
            if (string.IsNullOrEmpty(tb.Text))
            {
                MessageBox.Show($"Поле '{name}' не может быть пустым.", "ОШИБКА", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return false;
            }
            if (!double.TryParse(tb.Text, NumberStyles.Float, CultureInfo.InvariantCulture, out double value) ||
                    double.IsInfinity(value) || double.IsNaN(value) || value < 0 || value > 100)
            {
                MessageBox.Show($"В поле '{name}' должно быть число от 0 до 100.", "ОШИБКА", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return false;
            }
            return true;
        }
        private bool CheckCity(TextBox tb, string name)
        {
            if (string.IsNullOrEmpty(tb.Text))
            {
                MessageBox.Show($"Поле '{name}' не может быть пустым.", "ОШИБКА", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return false;
            }
            foreach (char c in tb.Text)
            {
                if (!char.IsLetter(c) && !char.IsWhiteSpace(c))
                {
                    MessageBox.Show($"Поле '{name}' может содержать только буквы и пробелы.", "ОШИБКА", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    return false;
                }
            }
            return true;
        }
        private void ClearInputFields()
        {
            txtPrice.Clear();
            txtDiscount.Clear();
            txtCity1.Clear();
            txtCity2.Clear();
        }

        private void LoadListFromFile(string filename)
        {
            try
            {
                if (!File.Exists(filename)) return;
                var lines = File.ReadAllLines(filename);
                foreach (var line in lines)
                {
                    if (!string.IsNullOrWhiteSpace(line))
                        tariff_list.Add(line.Trim());
                }
                ate.SetLists(tariff_list);
            }
            catch (TariffException ex)
            {
                MessageBox.Show(ex.Message, "ОШИБКА", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
            catch (Exception ex)
            {
                MessageBox.Show($"Ошибка при загрузке списка тарифов из файла: {ex.Message}.", "ОШИБКА", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }
        private void SaveListToFile(string filename)
        {
            try
            {
                File.WriteAllLines(filename, tariff_list);
            }
            catch (Exception ex)
            {
                MessageBox.Show($"Ошибка при сохранении списка тарифов в файл: {ex.Message}.", "ОШИБКА", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }
        private void LoadHistoryFromFile(string filename)
        {
            try
            {
                if (!File.Exists(filename)) return;
                var lines = File.ReadAllLines(filename);
                foreach (var line in lines)
                {
                    if (!string.IsNullOrWhiteSpace(line))
                        history.Add(line.Trim());
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show($"Ошибка при загрузке истории операций из файла: {ex.Message}.", "ОШИБКА", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }
        private void SaveHistoryToFile(string filename)
        {
            try
            {
                File.WriteAllLines(filename, history);
            }
            catch (Exception ex)
            {
                MessageBox.Show($"Ошибка при сохранении истории операций в файл: {ex.Message}.", "ОШИБКА", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }
    }
}
