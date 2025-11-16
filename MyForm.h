#ifndef MYFORM_H
#define MYFORM_H

#include <sstream>
#include <fstream>
#include <codecvt>
#include <msclr/marshal_cppstd.h>

#include "Class.h"
#include "Values.h"

namespace Exercise5 {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace msclr::interop;

	public ref class MyForm : public System::Windows::Forms::Form
	{
	private:
		System::ComponentModel::Container^ components;

		ATE* ate;
		
		System::Windows::Forms::TextBox^ txtPrice;
		System::Windows::Forms::TextBox^ txtDiscount;
		System::Windows::Forms::TextBox^ txtCity1;
		System::Windows::Forms::TextBox^ txtCity2;
		System::Windows::Forms::Button^ btnAddUsual;
		System::Windows::Forms::Button^ btnAddDiscount;
		System::Windows::Forms::Button^ btnCalculateAverage;
		System::Windows::Forms::Label^ lblPrice;
		System::Windows::Forms::Label^ lblDiscount;
		System::Windows::Forms::Label^ lblCity1;
		System::Windows::Forms::Label^ lblCity2;
		System::Windows::Forms::RichTextBox^ txtResult;
		System::Windows::Forms::Button^ btnShowList;
		System::Windows::Forms::Button^ btnShowHistory;
		System::Collections::Generic::List<System::String^>^ tariff_list;
		System::Collections::Generic::List<System::String^>^ history;

	public:
		MyForm(void);

	protected:
		~MyForm();

	private:
		bool CheckPrice(TextBox^ tb, String^ fn);
		bool CheckDiscount(TextBox^ tb, String^ fn);
		bool CheckCity(TextBox^ tb, String^ fn);
		void InitializeComponent(void);

		void btnAddUsual_Click(System::Object^ sender, System::EventArgs^ e);
		void btnAddDiscount_Click(System::Object^ sender, System::EventArgs^ e);
		void btnCalculateAverage_Click(System::Object^ sender, System::EventArgs^ e);

		void LoadListFromFile(const std::string& filename);
		void SaveListToFile(const std::string& filename);
		void LoadHistoryFromFile(const std::string& filename);
		void SaveHistoryToFile(const std::string& filename);

		System::Void btnShowList_Click(System::Object^ sender, System::EventArgs^ e);
		System::Void btnShowHistory_Click(System::Object^ sender, System::EventArgs^ e);

		void ClearInputFields();
	};
}

#endif