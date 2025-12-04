using System;
using System.Collections.Generic;
using System.IO;
using System.Reflection.Metadata.Ecma335;
using System.Windows.Forms;
using lab_5;

namespace lab_5
{
    public partial class Values : Form
    {
        private ATE Ate;
        private List<string> TariffList;
        private List<string> History;

        public Values(List<string> history)
        {
            History = new List<string>(history);
            InitializeHistory();
        }
        public Values(ATE ate, List<string> tariff_list)
        {
            Ate = ate;
            TariffList = new List<string>(tariff_list);
            InitializeList();
        }

        private void InitializeHistory()
        {
            var txtHistory = new RichTextBox()
            {
                Location = new Point(20, 20),
                Size = new Size(300, 300),
                ReadOnly = true,
                ScrollBars = RichTextBoxScrollBars.Vertical,
                Text = string.Join("\n", History)
            };
            var btnClose = new Button()
            {
                Location = new Point(20, 350),
                Size = new Size(300, 30),
                Text = "Закрыть историю операций"
            };
            btnClose.Click += new EventHandler((object sender, EventArgs e) => { Close(); });
            ClientSize = new Size(340, 400);
            Controls.AddRange(new Control[] { txtHistory, btnClose });
            Text = "История операций";
        }

        private DataGridView dgvTariffList;
        private OpenFileDialog ofdFile;
        private SaveFileDialog sfdFile;
        private void InitializeList()
        {
            dgvTariffList = new DataGridView()
            {
                Location = new System.Drawing.Point(20, 20),
                Size = new System.Drawing.Size(603, 300),
                ReadOnly = true,
                AllowUserToAddRows = false,
                RowHeadersVisible = false
            };
            ofdFile = new OpenFileDialog()
            { 
                Filter = "Text files *.txt | *.*",
                RestoreDirectory = true
            };
            sfdFile = new SaveFileDialog()
            {
                Filter = "Text files *.txt | *.*",
                RestoreDirectory = true
            };
            var btnClose = new Button()
            {
                Location = new Point(20, 350),
                Size = new Size(603, 30),
                Text = "Закрыть список тарифов"
            };
            btnClose.Click += new EventHandler((object sender, EventArgs e) => { Close(); });
            var btnLoadFromFile = new Button()
            {
                Location = new System.Drawing.Point(653, 20),
                Size = new System.Drawing.Size(300, 30),
                Text = "Загрузить тарифы из файла"
            };
            btnLoadFromFile.Click += btnLoadFromFile_Click;
            var btnSaveToFile = new Button()
            {
                Location = new System.Drawing.Point(653, 70),
                Size = new System.Drawing.Size(300, 30),
                Text = "Сохранить тарифы в файл"
            };
            btnSaveToFile.Click += btnSaveToFile_Click;
            var btnSortByPrice = new Button
            {
                Location = new System.Drawing.Point(653, 190),
                Size = new System.Drawing.Size(300, 30),
                Text = "Сортировать тарифы по цене"
            };
            btnSortByPrice.Click += btnSortByPrice_Click;
            var btnSortByCity1 = new Button
            {
                Location = new System.Drawing.Point(653, 240),
                Size = new System.Drawing.Size(300, 30),
                Text = "Сортировать тарифы по городу 1"
            };
            btnSortByCity1.Click += btnSortByCity1_Click;
            var btnSortByCity2 = new Button
            {
                Location = new System.Drawing.Point(653, 290),
                Size = new System.Drawing.Size(300, 30),
                Text = "Сортировать тарифы по городу 2"
            };
            btnSortByCity2.Click += btnSortByCity2_Click;
            SetListToGrid();
            ClientSize = new Size(973, 400);
            Controls.AddRange(new Control[] {dgvTariffList, btnClose, btnLoadFromFile, btnSaveToFile,
            btnSortByPrice, btnSortByCity1, btnSortByCity2});
            Text = "Список тарифов";
        }

        private void SetListToGrid()
        {
            dgvTariffList.Columns.Clear();
            dgvTariffList.Columns.Add("Index", "№");
            dgvTariffList.Columns.Add("Type", "Тип");
            dgvTariffList.Columns.Add("City1", "Город 1");
            dgvTariffList.Columns.Add("City2", "Город 2");
            dgvTariffList.Columns.Add("Price", "Цена");
            dgvTariffList.Columns.Add("Discount", "Скидка (%)");

            dgvTariffList.Rows.Clear();
            for (int i = 0; i < TariffList.Count; i++)
            {
                string[] parts = TariffList[i].Split(';');
                if (parts.Length < 4 || parts.Length > 5)
                {
                    MessageBox.Show("Неверный формат строки в списке тарифов.", "ОШИБКА", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    continue;
                }
                string type = parts[0] == "u" ? "обычный" : "льготный";
                string city1 = parts[1];
                string city2 = parts[2];
                string price = parts[3];
                string discount = (parts.Length == 5 && parts[0] == "d") ? parts[4] : "";
                dgvTariffList.Rows.Add((i + 1).ToString(), type, city1, city2, price, discount);
            }
        }
        private void GetListFromGrid()
        {
            TariffList.Clear();
            foreach (DataGridViewRow row in dgvTariffList.Rows)
            {
            if (row.IsNewRow) continue;
                string type = (row.Cells["Type"].Value.ToString() == "обычный") ? "u" : "d";
                string city1 = row.Cells["City1"].Value.ToString();
                string city2 = row.Cells["City2"].Value.ToString();
                string price = row.Cells["Price"].Value.ToString();
                string discount = row.Cells["Discount"].Value.ToString();
                string line = $"{type};{city1};{city2};{price}";
                if (type == "d" && !string.IsNullOrWhiteSpace(discount))
                    line += $";{discount}";
                TariffList.Add(line);
            }
        }
        private void btnLoadFromFile_Click(object sender, EventArgs e)
        {
            if (ofdFile.ShowDialog() == DialogResult.OK)
            {
                try
                {
                    var lines = File.ReadAllLines(ofdFile.FileName);
                    TariffList = lines.Where(l => !string.IsNullOrWhiteSpace(l)).ToList();
                    Ate.SetLists(TariffList);
                    SetListToGrid();
                    MessageBox.Show("Тарифы успешно загружены из файла.", "УСПЕХ", MessageBoxButtons.OK, MessageBoxIcon.Information);
                }
                catch (Exception ex)
                {
                    MessageBox.Show($"Ошибка при загрузке тарифов из файла: {ex.Message}.", "ОШИБКА", MessageBoxButtons.OK, MessageBoxIcon.Error);
                }
            }
        }
        private void btnSaveToFile_Click(object sender, EventArgs e)
        {
            try
            {
                if (sfdFile.ShowDialog() == DialogResult.OK)
                {
                    GetListFromGrid();
                    File.WriteAllLines(sfdFile.FileName, TariffList);
                    MessageBox.Show("Тарифы сохранены в файл.", "УСПЕХ", MessageBoxButtons.OK, MessageBoxIcon.Information);
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show($"Ошибка при сохранении тарифов в файл: {ex.Message}.", "ОШИБКА", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }
        private void btnSortByPrice_Click(object sender, EventArgs e)
        {
            TariffList = Ate.SortByPrice(TariffList);
            SetListToGrid();
        }
        private void btnSortByCity1_Click(object sender, EventArgs e)
        {
            TariffList = Ate.SortByCity1(TariffList);
            SetListToGrid();
        }
        private void btnSortByCity2_Click(object sender, EventArgs e)
        {
            TariffList = Ate.SortByCity2(TariffList);
            SetListToGrid();
        }
    }
}