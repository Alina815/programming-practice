#include "MyForm.h"
#include <Windows.h>

using namespace Exercise5;
using namespace System;
using namespace System::Windows::Forms;

[STAThread]
void main(array<String^>^ arg) {
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);

	Exercise5::MyForm from;
	Application::Run(% from);
}