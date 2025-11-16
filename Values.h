#ifndef VALUES_H
#define VALUES_H

#include <sstream>
#include <fstream>
#include <codecvt>

#include "Class.h"

namespace Exercise5 {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace msclr::interop;

	public ref class Values : public System::Windows::Forms::Form
	{
	public:
		Values();
		Values(ATE* atesystem);
		void SetHistory(System::Collections::Generic::List<System::String^>^ History);
		void SetList(System::Collections::Generic::List<System::String^>^ tariff_list);
		System::Collections::Generic::List<System::String^>^ GetList();
		System::Collections::Generic::List<System::String^>^ GetHistory();
		void LoadListFromFile(const std::string& filename);
		void LoadHistoryFromFile(const std::string& filename);
		void SaveListToFile(const std::string& filename);
		void SaveHistoryToFile(const std::string& filename);

	protected:
		~Values();

	private:
		ATE* ate;

		System::ComponentModel::Container ^components;

		System::Windows::Forms::RichTextBox^ txtHistory;
		System::Windows::Forms::DataGridView^ dgvTariffList;
		System::Windows::Forms::OpenFileDialog^ opnFile;

		System::Windows::Forms::Button^ btnClose;
		System::Windows::Forms::Button^ btnLoadFromFile;
		System::Windows::Forms::Button^ btnSortByPrice;
		System::Windows::Forms::Button^ btnSortByCity1;
		System::Windows::Forms::Button^ btnSortByCity2;

		void InitializeHistory(void);
		void InitializeTariffList(void);

		System::Void btnClose_Click(System::Object^ sender, System::EventArgs^ e);
		System::Void btnLoadFromFile_Click(System::Object^ sender, System::EventArgs^ e);
		System::Void btnSortByPrice_Click(System::Object^ sender, System::EventArgs^ e);
		System::Void btnSortByCity1_Click(System::Object^ sender, System::EventArgs^ e);
		System::Void btnSortByCity2_Click(System::Object^ sender, System::EventArgs^ e);
	};
}

#endif