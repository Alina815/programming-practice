#include "Values.h"

namespace Exercise5 {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace msclr::interop;

	Values::Values()
	{
		InitializeHistory();
		ate = nullptr;
	}
	Values::Values(ATE* atesystem) {
		InitializeTariffList();
		ate = atesystem;
	}

	void Values::SetHistory(System::Collections::Generic::List<System::String^>^ History) {
		if (txtHistory == nullptr) {
			MessageBox::Show("Поле вывода не инициализировано!", "КРИТИЧЕСКАЯ ОШИБКА", MessageBoxButtons::OK, MessageBoxIcon::Error);
			return;
		}
		txtHistory->Clear();
		for (int i = 0; i < History->Count; i++) {
			txtHistory->Text += History[i] + L"\n";
		}
	}

	void Values::SetList(System::Collections::Generic::List<System::String^>^ tariff_list) {
		if (dgvTariffList == nullptr) {
			MessageBox::Show("Поле вывода не инициализировано!", "КРИТИЧЕСКАЯ ОШИБКА", MessageBoxButtons::OK, MessageBoxIcon::Error);
			return;
		}
		dgvTariffList->Rows->Clear();
		dgvTariffList->Columns->Clear();
		dgvTariffList->Columns->Add("Index", "№");
		dgvTariffList->Columns->Add("Type", "Тип");
		dgvTariffList->Columns->Add("City1", "Город 1");
		dgvTariffList->Columns->Add("City2", "Город 2");
		dgvTariffList->Columns->Add("Price", "Цена");
		dgvTariffList->Columns->Add("Discount", "Скидка (%)");
		for (int i = 0; i < tariff_list->Count; i++) {
			array<System::String^>^ parts = tariff_list[i]->Split(gcnew array<System::Char>{L';'}, StringSplitOptions::None);
			if (parts->Length < 4 || parts->Length > 5)
				MessageBox::Show("Неверный формат строк в списке тарифов.", "ОШИБКА", MessageBoxButtons::OK, MessageBoxIcon::Error);
			System::String^ type = (parts[0] == L"u") ? L"обычный" : L"льготный";
			System::String^ city1 = parts[1];
			System::String^ city2 = parts[2];
			System::String^ price = parts[3];
			System::String^ discount = (parts->Length == 5 && parts[0] == L"d") ? parts[4] : L"";
			array<System::String^>^ row = { (i + 1).ToString(), type, city1, city2, price, discount };
			dgvTariffList->Rows->Add(row);
		}
	}

	System::Collections::Generic::List<System::String^>^ Values::GetList() {
		if (dgvTariffList == nullptr) {
			MessageBox::Show("Поле вывода не инициализировано!", "КРИТИЧЕСКАЯ ОШИБКА", MessageBoxButtons::OK, MessageBoxIcon::Error);
			return gcnew System::Collections::Generic::List<System::String^>();
		}
		System::Collections::Generic::List<System::String^>^ tariff_list = gcnew System::Collections::Generic::List<System::String^>();
		for (int i = 0; i < dgvTariffList->Rows->Count; i++) {
			System::Windows::Forms::DataGridViewRow^ row = dgvTariffList->Rows[i];
			try {
				System::String^ type = (safe_cast<String^>(row->Cells[1]->Value) == "обычный") ? "u" : "d";
				System::String^ city1 = safe_cast<System::String^>(row->Cells[2]->Value);
				System::String^ city2 = safe_cast<System::String^>(row->Cells[3]->Value);
				System::String^ price = safe_cast<System::String^>(row->Cells[4]->Value);
				System::String^ discount = safe_cast<System::String^>(row->Cells[5]->Value);
				System::String^ line = type + ";" + city1 + ";" + city2 + ";" + price;
				if (type == "d" && discount != nullptr && !System::String::IsNullOrWhiteSpace(discount))
					line += ";" + discount;
				tariff_list->Add(line);
			}
			catch (System::Exception^ ex) {
				MessageBox::Show("ошибка при извлечении данных из таблицы тарифов: " + ex->Message, "ОШИБКА", MessageBoxButtons::OK, MessageBoxIcon::Error);
			}
		}
		return tariff_list;
	}

	System::Collections::Generic::List<System::String^>^ Values::GetHistory() {
		if (txtHistory == nullptr) {
			MessageBox::Show("Поле вывода не инициализировано!", "КРИТИЧЕСКАЯ ОШИБКА", MessageBoxButtons::OK, MessageBoxIcon::Error);
			return gcnew System::Collections::Generic::List<System::String^>();
		}
		System::Collections::Generic::List<System::String^>^ history = gcnew System::Collections::Generic::List<System::String^>();
		array<System::String^>^ lines = txtHistory->Text->Split('\n');
		for each (System::String ^ line in lines) {
			if (!String::IsNullOrWhiteSpace(line)) {
				history->Add(line);
			}
		}
		return history;
	}

	void Values::LoadListFromFile(const std::string& filename) {
		try {
			array<System::String^>^ lines = System::IO::File::ReadAllLines(gcnew System::String(filename.c_str()), System::Text::Encoding::UTF8);
			System::Collections::Generic::List<System::String^>^ tariff_list = gcnew System::Collections::Generic::List<System::String^>();
			for each (String ^ line in lines) {
				if (!System::String::IsNullOrWhiteSpace(line))
					tariff_list->Add(line);
			}
			SetList(tariff_list);
			ate->SetLists(tariff_list);
		}
		catch (System::IO::FileNotFoundException^) {
			MessageBox::Show("Файл списка тарифов не найден.", "ИНФО", MessageBoxButtons::OK, MessageBoxIcon::Information);
		}
		catch (System::Exception^ ex) {
			MessageBox::Show("ошибка при загрузке списка тарифов из файла: " + ex->Message, "ОШИБКА", MessageBoxButtons::OK, MessageBoxIcon::Error);
		}
	}

	void Values::LoadHistoryFromFile(const std::string& filename) {
		try {
			array<System::String^>^ lines = System::IO::File::ReadAllLines(gcnew System::String(filename.c_str()), System::Text::Encoding::UTF8);
			System::Collections::Generic::List<System::String^>^ history = gcnew System::Collections::Generic::List<System::String^>();
			for each (System::String ^ line in lines) {
				if (!System::String::IsNullOrWhiteSpace(line))
					history->Add(line);
			}
			SetHistory(history);
		}
		catch (System::IO::FileNotFoundException^) {
			MessageBox::Show("Файл истории не найден.", "ИНФО", MessageBoxButtons::OK, MessageBoxIcon::Information);
		}
		catch (System::DllNotFoundException^ ex) {
			MessageBox::Show("ошибка при загрузке истории из файла: " + ex->Message, "ОШИБКА", MessageBoxButtons::OK, MessageBoxIcon::Error);
		}
	}

	void Values::SaveListToFile(const std::string& filename) {
		try {
			System::Collections::Generic::List<System::String^>^ tariff_list = GetList();
			array<System::String^>^ lines = tariff_list->ToArray();
			System::IO::File::WriteAllLines(gcnew System::String(filename.c_str()), lines, System::Text::Encoding::UTF8);
		}
		catch (System::IO::FileNotFoundException^) {
			MessageBox::Show("Файл списка тарифов не найден.", "ИНФО", MessageBoxButtons::OK, MessageBoxIcon::Information);
		}
		catch (System::Exception^ ex) {
			MessageBox::Show("ошибка при сохранении списка тарифов в файл: " + ex->Message, "ОШИБКА", MessageBoxButtons::OK, MessageBoxIcon::Error);
		}
	}

	void Values::SaveHistoryToFile(const std::string& filename) {
		try {
			System::Collections::Generic::List<System::String^>^ history = GetHistory();
			array<System::String^>^ lines = history->ToArray();
			System::IO::File::WriteAllLines(gcnew System::String(filename.c_str()), lines, System::Text::Encoding::UTF8);
		}
		catch (System::IO::FileNotFoundException^) {
			MessageBox::Show("Файл истории не найден.", "ИНФО", MessageBoxButtons::OK, MessageBoxIcon::Information);
		}
		catch (System::Exception^ ex) {
			MessageBox::Show("ошибка при сохранении истории в файл: " + ex->Message, "ОШИБКА", MessageBoxButtons::OK, MessageBoxIcon::Error);
		}
	}

	Values::~Values()
	{
		if (components)
			delete components;
	}

	void Values::InitializeHistory(void)
	{
		this->txtHistory = (gcnew System::Windows::Forms::RichTextBox());
		this->btnClose = (gcnew System::Windows::Forms::Button());

		this->txtHistory->Location = System::Drawing::Point(20, 20);
		this->txtHistory->Name = L"txtHistory";
		this->txtHistory->ReadOnly = true;
		this->txtHistory->ScrollBars = System::Windows::Forms::RichTextBoxScrollBars::Vertical;
		this->txtHistory->Size = System::Drawing::Size(300, 300);
		this->txtHistory->Text = L"";

		this->btnClose->Location = System::Drawing::Point(20, 350);
		this->btnClose->Name = L"btnClose";
		this->btnClose->Size = System::Drawing::Size(300, 30);
		this->btnClose->Text = L"Закрыть историю операций";
		this->btnClose->Click += gcnew System::EventHandler(this, &Values::btnClose_Click);

		this->ClientSize = System::Drawing::Size(340, 400);
		this->Controls->Add(this->txtHistory);
		this->Controls->Add(this->btnClose);
		this->Name = L"History";
		this->Text = L"История операций";
	}

	void Values::InitializeTariffList(void)
	{
		this->dgvTariffList = (gcnew System::Windows::Forms::DataGridView());
		this->opnFile = (gcnew System::Windows::Forms::OpenFileDialog());
		this->btnClose = (gcnew System::Windows::Forms::Button());
		this->btnSortByPrice = gcnew Button();
		this->btnSortByCity1 = gcnew Button();
		this->btnSortByCity2 = gcnew Button();
		this->btnLoadFromFile = gcnew Button();

		this->dgvTariffList->Location = System::Drawing::Point(20, 20);
		this->dgvTariffList->Name = L"dgvTariffList";
		this->dgvTariffList->ReadOnly = true;
		this->dgvTariffList->Size = System::Drawing::Size(603, 300);
		this->dgvTariffList->Text = L"";
		this->dgvTariffList->AllowUserToAddRows = false;
		this->dgvTariffList->RowHeadersVisible = false;

		this->btnClose->Location = System::Drawing::Point(20, 350);
		this->btnClose->Name = L"btnClose";
		this->btnClose->Size = System::Drawing::Size(603, 30);
		this->btnClose->Text = L"Закрыть список тарифов";
		this->btnClose->Click += gcnew System::EventHandler(this, &Values::btnClose_Click);
		this->btnSortByPrice->TabIndex = 4;

		this->opnFile->Filter = "Text files (*.txt)|*.txt|All files (*.*)|*.*";
		this->opnFile->FilterIndex = 1;
		this->opnFile->RestoreDirectory = true;

		this->btnLoadFromFile->Location = System::Drawing::Point(653, 20);
		this->btnLoadFromFile->Name = L"btnLoadFromFile";
		this->btnLoadFromFile->Size = System::Drawing::Size(300, 30);
		this->btnLoadFromFile->Text = L"Загрузить тарифы из файла";
		this->btnLoadFromFile->Click += gcnew System::EventHandler(this, &Values::btnLoadFromFile_Click);
		this->btnLoadFromFile->TabIndex = 0;

		this->btnSortByPrice->Location = System::Drawing::Point(653, 190);
		this->btnSortByPrice->Name = L"btnSortByPrice";
		this->btnSortByPrice->Size = System::Drawing::Size(300, 30);
		this->btnSortByPrice->Text = L"Сортировать тарифы по цене";
		this->btnSortByPrice->Click += gcnew System::EventHandler(this, &Values::btnSortByPrice_Click);
		this->btnSortByPrice->TabIndex = 1;

		this->btnSortByCity1->Location = System::Drawing::Point(653, 240);
		this->btnSortByCity1->Name = L"btnSortByCity1";
		this->btnSortByCity1->Size = System::Drawing::Size(300, 30);
		this->btnSortByCity1->Text = L"Сортировать тарифы по городу 1";
		this->btnSortByCity1->Click += gcnew System::EventHandler(this, &Values::btnSortByCity1_Click);
		this->btnSortByPrice->TabIndex = 2;

		this->btnSortByCity2->Location = System::Drawing::Point(653, 290);
		this->btnSortByCity2->Name = L"btnSortByCity2";
		this->btnSortByCity2->Size = System::Drawing::Size(300, 30);
		this->btnSortByCity2->Text = L"Сортировать тарифы по городу 2";
		this->btnSortByCity2->Click += gcnew System::EventHandler(this, &Values::btnSortByCity2_Click);
		this->btnSortByPrice->TabIndex = 3;

		this->ClientSize = System::Drawing::Size(973, 400);
		this->Controls->Add(this->dgvTariffList);
		this->Controls->Add(this->btnClose);
		this->Controls->Add(this->btnLoadFromFile);
		this->Controls->Add(this->btnSortByPrice);
		this->Controls->Add(this->btnSortByCity1);
		this->Controls->Add(this->btnSortByCity2);
		this->Name = L"TarifFList";
		this->Text = L"Список тарифов";
	}

	System::Void Values::btnClose_Click(System::Object^ sender, System::EventArgs^ e) {
		this->Close();
	}

	System::Void Values::btnLoadFromFile_Click(System::Object^ sender, System::EventArgs^ e) {
		System::Windows::Forms::DialogResult result = opnFile->ShowDialog();
		if (result == System::Windows::Forms::DialogResult::OK) {
			System::String^ file_path = opnFile->FileName;
			try {
				array<System::String^>^ lines = System::IO::File::ReadAllLines(file_path, System::Text::Encoding::UTF8);
				System::Collections::Generic::List<System::String^>^ temp_tariff_list = gcnew System::Collections::Generic::List<System::String^>();
				for each (System::String ^ line in lines) {
					if (!System::String::IsNullOrWhiteSpace(line)) {
						temp_tariff_list->Add(line);
					}
				}
				if (ate != nullptr) {
					ate->SetLists(temp_tariff_list);
					SetList(temp_tariff_list);
					MessageBox::Show("Тарифы успешно загружены из файла.", "УСПЕХ", MessageBoxButtons::OK, MessageBoxIcon::Information);
				}
				else {
					MessageBox::Show("Система тарифов не инициализирована.", "ОШИБКА", MessageBoxButtons::OK, MessageBoxIcon::Error);
				}

			}
			catch (System::IO::FileNotFoundException^) {
				MessageBox::Show("Файл не найден.", "ОШИБКА", MessageBoxButtons::OK, MessageBoxIcon::Error);
			}
			catch (System::Exception^ ex) {
				MessageBox::Show("ошибка при загрузке файла: " + ex->Message, "ОШИБКА", MessageBoxButtons::OK, MessageBoxIcon::Error);
			}
		}
	}

	System::Void Values::btnSortByPrice_Click(System::Object^ sender, System::EventArgs^ e) {
		SetList(ate->SortTariffsByPrice(GetList()));
	}

	System::Void Values::btnSortByCity1_Click(System::Object^ sender, System::EventArgs^ e) {
		SetList(ate->SortTariffsByCity1(GetList()));
	}

	System::Void Values::btnSortByCity2_Click(System::Object^ sender, System::EventArgs^ e) {
		SetList(ate->SortTariffsByCity2(GetList()));
	}
}