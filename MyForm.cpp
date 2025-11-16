#include "MyForm.h"

namespace Exercise5 {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace msclr::interop;
	using namespace System::IO;
	using namespace System::Text;

	MyForm::MyForm(void)
	{
		InitializeComponent();
		ate = new ATE();
		tariff_list = gcnew System::Collections::Generic::List<System::String^>();
		LoadListFromFile("tariffs.txt");
		history = gcnew System::Collections::Generic::List<System::String^>();
		LoadHistoryFromFile("history.txt");
	}
	MyForm::~MyForm()
			{
		SaveListToFile("tariffs.txt");
		SaveHistoryToFile("history.txt");
				if (components)
					delete components;
				delete ate;
				delete tariff_list;
				delete history;
			}
	void MyForm::InitializeComponent(void)
	{
		this->txtPrice = (gcnew System::Windows::Forms::TextBox());
		this->txtDiscount = (gcnew System::Windows::Forms::TextBox());
		this->txtCity1 = (gcnew System::Windows::Forms::TextBox());
		this->txtCity2 = (gcnew System::Windows::Forms::TextBox());
		this->btnAddUsual = (gcnew System::Windows::Forms::Button());
		this->btnAddDiscount = (gcnew System::Windows::Forms::Button());
		this->btnCalculateAverage = (gcnew System::Windows::Forms::Button());
		this->lblPrice = (gcnew System::Windows::Forms::Label());
		this->lblDiscount = (gcnew System::Windows::Forms::Label());
		this->lblCity1 = (gcnew System::Windows::Forms::Label());
		this->lblCity2 = (gcnew System::Windows::Forms::Label());
		this->txtResult = (gcnew System::Windows::Forms::RichTextBox());
		this->btnShowList = (gcnew System::Windows::Forms::Button());
		this->btnShowHistory = (gcnew System::Windows::Forms::Button());
		this->SuspendLayout();
		// 
		// txtPrice
		// 
		this->txtPrice->Location = System::Drawing::Point(180, 20);
		this->txtPrice->Name = L"txtPrice";
		this->txtPrice->Size = System::Drawing::Size(100, 22);
		this->txtPrice->TabIndex = 1;
		// 
		// txtDiscount
		// 
		this->txtDiscount->Location = System::Drawing::Point(180, 50);
		this->txtDiscount->Name = L"txtDiscount";
		this->txtDiscount->Size = System::Drawing::Size(100, 22);
		this->txtDiscount->TabIndex = 2;
		// 
		// txtCity1
		// 
		this->txtCity1->Location = System::Drawing::Point(180, 80);
		this->txtCity1->Name = L"txtCity1";
		this->txtCity1->Size = System::Drawing::Size(100, 22);
		this->txtCity1->TabIndex = 3;
		// 
		// txtCity2
		// 
		this->txtCity2->Location = System::Drawing::Point(180, 110);
		this->txtCity2->Name = L"txtCity2";
		this->txtCity2->Size = System::Drawing::Size(100, 22);
		this->txtCity2->TabIndex = 4;
		// 
		// btnAddUsual
		// 
		this->btnAddUsual->Location = System::Drawing::Point(20, 230);
		this->btnAddUsual->Name = L"btnAddUsual";
		this->btnAddUsual->Size = System::Drawing::Size(260, 25);
		this->btnAddUsual->TabIndex = 5;
		this->btnAddUsual->Text = L"Добавить обычный тариф";
		this->btnAddUsual->Click += gcnew System::EventHandler(this, &MyForm::btnAddUsual_Click);
		// 
		// btnAddDiscount
		// 
		this->btnAddDiscount->Location = System::Drawing::Point(20, 270);
		this->btnAddDiscount->Name = L"btnAddDiscount";
		this->btnAddDiscount->Size = System::Drawing::Size(260, 25);
		this->btnAddDiscount->TabIndex = 5;
		this->btnAddDiscount->Text = L"Добавить льготный тариф";
		this->btnAddDiscount->Click += gcnew System::EventHandler(this, &MyForm::btnAddDiscount_Click);
		// 
		// btnCalculateAverage
		// 
		this->btnCalculateAverage->Location = System::Drawing::Point(20, 310);
		this->btnCalculateAverage->Name = L"btnCalculateAverage";
		this->btnCalculateAverage->Size = System::Drawing::Size(260, 25);
		this->btnCalculateAverage->TabIndex = 6;
		this->btnCalculateAverage->Text = L"Вычислить среднюю цену";
		this->btnCalculateAverage->Click += gcnew System::EventHandler(this, &MyForm::btnCalculateAverage_Click);
		// 
		// lblPrice
		// 
		this->lblPrice->Location = System::Drawing::Point(20, 20);
		this->lblPrice->Name = L"lblPrice";
		this->lblPrice->Size = System::Drawing::Size(100, 20);
		this->lblPrice->Text = L"Цена:";
		// 
		// lblDiscount
		// 
		this->lblDiscount->Location = System::Drawing::Point(20, 50);
		this->lblDiscount->Name = L"lblDiscount";
		this->lblDiscount->Size = System::Drawing::Size(100, 20);
		this->lblDiscount->Text = L"Скидка (%):";
		// 
		// lblCity1
		// 
		this->lblCity1->Location = System::Drawing::Point(20, 80);
		this->lblCity1->Name = L"lblCity1";
		this->lblCity1->Size = System::Drawing::Size(100, 20);
		this->lblCity1->Text = L"Город 1:";
		// 
		// lblCity2
		// 
		this->lblCity2->Location = System::Drawing::Point(20, 110);
		this->lblCity2->Name = L"lblCity2";
		this->lblCity2->Size = System::Drawing::Size(100, 20);
		this->lblCity2->Text = L"Город 2:";
		// 
		// txtResult
		// 
		this->txtResult->Location = System::Drawing::Point(375, 20);
		this->txtResult->Name = L"txtResult";
		this->txtResult->ReadOnly = true;
		this->txtResult->Size = System::Drawing::Size(215, 315);
		this->txtResult->Text = L"";
		// 
		// btnShowList
		// 
		this->btnShowList->Location = System::Drawing::Point(375, 370);
		this->btnShowList->Name = L"btnShowList";
		this->btnShowList->Size = System::Drawing::Size(215, 30);
		this->btnShowList->TabIndex = 7;
		this->btnShowList->Text = L"Показать список тарифов";
		this->btnShowList->Click += gcnew System::EventHandler(this, &MyForm::btnShowList_Click);
		// 
		// btnShowHistory
		// 
		this->btnShowHistory->Location = System::Drawing::Point(375, 420);
		this->btnShowHistory->Name = L"btnShowHistory";
		this->btnShowHistory->Size = System::Drawing::Size(215, 30);
		this->btnShowHistory->TabIndex = 8;
		this->btnShowHistory->Text = L"Показать историю операций";
		this->btnShowHistory->Click += gcnew System::EventHandler(this, &MyForm::btnShowHistory_Click);		
		// 
		// MyForm
		// 
		this->ClientSize = System::Drawing::Size(610, 470);
		this->Controls->Add(this->lblPrice);
		this->Controls->Add(this->lblDiscount);
		this->Controls->Add(this->lblCity1);
		this->Controls->Add(this->lblCity2);
		this->Controls->Add(this->txtPrice);
		this->Controls->Add(this->txtDiscount);
		this->Controls->Add(this->txtCity1);
		this->Controls->Add(this->txtCity2);
		this->Controls->Add(this->btnAddUsual);
		this->Controls->Add(this->btnAddDiscount);
		this->Controls->Add(this->btnCalculateAverage);
		this->Controls->Add(this->btnShowList);
		this->Controls->Add(this->txtResult);
		this->Controls->Add(this->btnShowHistory);
		this->Name = L"MyForm";
		this->Text = L"Система управления тарифами";
		this->ResumeLayout(false);
		this->PerformLayout();

	}

	void MyForm::btnAddUsual_Click(System::Object^ sender, System::EventArgs^ e) {
		try {
			if (!CheckPrice(txtPrice, "Цена"))
				return;
			if (!CheckCity(txtCity1, "Город 1"))
				return;
			if (!CheckCity(txtCity2, "Город 2"))
				return;
			double price = Convert::ToDouble(txtPrice->Text);
			String^ city1 = txtCity1->Text;
			String^ city2 = txtCity2->Text;
			std::string c1 = marshal_as<std::string>(city1);
			std::string c2 = marshal_as<std::string>(city2);
			ate->add_usual_tariff(price, c1, c2);
			MessageBox::Show("Обычный тариф добавлен!", "УСПЕХ", MessageBoxButtons::OK, MessageBoxIcon::Information);
			ClearInputFields();
			tariff_list->Add(L"u;" + city1 + L";" + city2 + L";" + price.ToString("F2"));
			history->Add(L"Добавлен обычный тариф: " + city1 + L" -> " + city2 + L" = " + price.ToString("F2") + L".");
		}
		catch (TariffException& ex) {
			String^ x = msclr::interop::marshal_as<String^>(std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(static_cast<std::string>(ex.what())));
			MessageBox::Show(x, "ОШИБКА", MessageBoxButtons::OK, MessageBoxIcon::Error);
		}
		catch (...) {
			MessageBox::Show("Неверный формат данных.", "ОШИБКА ВВОДА", MessageBoxButtons::OK, MessageBoxIcon::Error);
		}
	}

	void MyForm::btnAddDiscount_Click(System::Object^ sender, System::EventArgs^ e) {
		try {
			if (!CheckPrice(txtPrice, "Цена"))
				return;
			if (!CheckDiscount(txtDiscount, "Скидка (%)"))
				return;
			if (!CheckCity(txtCity1, "Город 1"))
				return;
			if (!CheckCity(txtCity2, "Город 2"))
				return;
			double price = Convert::ToDouble(txtPrice->Text);
			double discount = Convert::ToDouble(txtDiscount->Text);
			String^ city1 = txtCity1->Text;
			String^ city2 = txtCity2->Text;
			std::string c1 = marshal_as<std::string>(city1);
			std::string c2 = marshal_as<std::string>(city2);
			ate->add_discount_tariff(price, discount, c1, c2);
			MessageBox::Show("Льготный тариф добавлен!", "УСПЕХ",
				MessageBoxButtons::OK, MessageBoxIcon::Information);
			ClearInputFields();
			tariff_list->Add(L"d;" + city1 + L";" + city2 + L";" + price.ToString("F2") + L";" + discount.ToString("F2"));
			history->Add(L"Добавлен льготный тариф: " + city1 + L" -> " + city2 + L" = " + price.ToString("F2") + L" % " + discount.ToString("F2") + L".");
		}
		catch (TariffException& ex) {
			String^ x = msclr::interop::marshal_as<String^>(std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(static_cast<std::string>(ex.what())));
			MessageBox::Show(x, "ОШИБКА", MessageBoxButtons::OK, MessageBoxIcon::Error);
		}
		catch (...) {
			MessageBox::Show("Неверный формат данных.", "ОШИБКА ВВОДА", MessageBoxButtons::OK, MessageBoxIcon::Error);
		}
	}

	void MyForm::btnCalculateAverage_Click(System::Object^ sender, System::EventArgs^ e) {
		try {
			double average = ate->calculate_average_price();
			txtResult->Text = "средняя цена тарифов = " + average.ToString("F2");
			history->Add(L"Вычислена средняя цена тарифов = " + average.ToString("F2") + L".");
		}
		catch (TariffException& ex) {
			String^ x = msclr::interop::marshal_as<String^>(std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(static_cast<std::string>(ex.what())));
			MessageBox::Show(x, "ОШИБКА", MessageBoxButtons::OK, MessageBoxIcon::Error);
		}
	}

	void MyForm::LoadListFromFile(const std::string& filename) {
		try {
			array<System::String^>^ lines = System::IO::File::ReadAllLines(gcnew System::String(filename.c_str()));
			for each (String^ line in lines) {
				if (!System::String::IsNullOrWhiteSpace(line))
					tariff_list->Add(line);
			}
			ate->SetLists(tariff_list);
		}
		catch (TariffException& ex) {
			String^ x = msclr::interop::marshal_as<String^>(std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(static_cast<std::string>(ex.what())));
			MessageBox::Show(x, "ОШИБКА", MessageBoxButtons::OK, MessageBoxIcon::Error);
		}
		catch (System::IO::FileNotFoundException^) {
			MessageBox::Show("Файл списка тарифов не найден, начинаем с пустого списка тарифов.", "ИНФО", MessageBoxButtons::OK, MessageBoxIcon::Information);
		}
		catch (...) {
			MessageBox::Show("Ошибка при загрузке списка тарифов из файла.", "ОШИБКА", MessageBoxButtons::OK, MessageBoxIcon::Error);
		}
	}

	void MyForm::LoadHistoryFromFile(const std::string& filename) {
		try {
			array<System::String^>^ lines = System::IO::File::ReadAllLines(gcnew System::String(filename.c_str()));
			for each (String ^ line in lines) {
				if (!System::String::IsNullOrWhiteSpace(line))
					history->Add(line);
			}
		}
		catch (System::IO::FileNotFoundException^) {
			MessageBox::Show("Файл истории не найден, начинаем с пустой истории.", "ИНФО", MessageBoxButtons::OK, MessageBoxIcon::Information);
		}
		catch (...) {
			MessageBox::Show("Ошибка при загрузке истории из файла.", "ОШИБКА", MessageBoxButtons::OK, MessageBoxIcon::Error);
		}
	}

	void MyForm::SaveListToFile(const std::string& filename) {
		try {
			array<System::String^>^ lines = tariff_list->ToArray();
			System::IO::File::WriteAllLines(gcnew System::String(filename.c_str()), lines);
		}
		catch (...) {
			MessageBox::Show("Ошибка при сохранении списка тарифов в файл.", "ОШИБКА", MessageBoxButtons::OK, MessageBoxIcon::Error);
		}
	}

	void MyForm::SaveHistoryToFile(const std::string& filename) {
		try {
			array<System::String^>^ lines = history->ToArray();
			System::IO::File::WriteAllLines(gcnew System::String(filename.c_str()), lines);
		}
		catch (...) {
			MessageBox::Show("Ошибка при сохранении истории в файл.", "ОШИБКА", MessageBoxButtons::OK, MessageBoxIcon::Error);
		}
	}

	System::Void MyForm::btnShowList_Click(System::Object^ sender, System::EventArgs^ e) {
		Values^ List = gcnew Values(ate);
		List->SetList(tariff_list);
		List->ShowDialog();
	}

	System::Void MyForm::btnShowHistory_Click(System::Object^ sender, System::EventArgs^ e) {
		Values^ History = gcnew Values();
		History->SetHistory(history);
		History->ShowDialog();
	}

	bool MyForm::CheckPrice(TextBox^ tb, String^ fn) {
		if (String::IsNullOrWhiteSpace(tb->Text)) {
			MessageBox::Show(("Поле '" + fn + "' не может быть пустым."), "ОШИБКА", MessageBoxButtons::OK, MessageBoxIcon::Error);
			return false;
		}
		double value;
		if (!Double::TryParse(tb->Text, value)) {
			MessageBox::Show(("В поле '" + fn + "' должно быть введено вещественное значение."), "ОШИБКА", MessageBoxButtons::OK, MessageBoxIcon::Error);
			return false;
		}
		if (Double::IsInfinity(value) || Double::IsNaN(value)) {
			MessageBox::Show(("Значение в поле '" + fn + "' выходит за допустимый диапазон."), "ОШИБКА", MessageBoxButtons::OK, MessageBoxIcon::Error);
			return false;
		}
		return true;
	}

	bool MyForm::CheckDiscount(TextBox^ tb, String^ fn) {
		if (String::IsNullOrWhiteSpace(tb->Text)) {
			MessageBox::Show(("Поле '" + fn + "' не может быть пустым."), "ОШИБКА", MessageBoxButtons::OK, MessageBoxIcon::Error);
			return false;
		}
		double value;
		if (!Double::TryParse(tb->Text, value)) {
			MessageBox::Show(("В поле '" + fn + "' должно быть введено вещественное значение."), "ОШИБКА", MessageBoxButtons::OK, MessageBoxIcon::Error);
			return false;
		}
		if (Double::IsInfinity(value) || Double::IsNaN(value)) {
			MessageBox::Show(("Значение в поле '" + fn + "' выходит за допустимый диапазон."), "ОШИБКА", MessageBoxButtons::OK, MessageBoxIcon::Error);
			return false;
		}
		return true;
	}

	bool MyForm::CheckCity(TextBox^ tb, String^ fn) {
		if (String::IsNullOrWhiteSpace(tb->Text)) {
			MessageBox::Show(("Поле '" + fn + "' не может быть пустым."), "ОШИБКА", MessageBoxButtons::OK, MessageBoxIcon::Error);
			return false;
		}
		String^ value = tb->Text;
		for each (wchar_t c in value) {
			if (!Char::IsLetter(c) && !Char::IsWhiteSpace(c)) {
				MessageBox::Show(("Поле '" + fn + "' может содержать только буквы и пробелы."), "ОШИБКА", MessageBoxButtons::OK, MessageBoxIcon::Error);
				return false;
			}
		}
		return true;
	}

	void MyForm::ClearInputFields() {
		txtPrice->Text = "";
		txtDiscount->Text = "";
		txtCity1->Text = "";
		txtCity2->Text = "";
	}
}