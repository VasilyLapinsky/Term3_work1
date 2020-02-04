// Term3_Task1.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <vector>
#include <string>
#include <exception>
#include <map>
#include <fstream>
using namespace std;

const int size_mas = 5;// фиксируем размер массива
const int m = 4; // длина идентификатора
vector<char> sys_symbols{ '=',';','+','-','/','*','(',')',',' };// сиволы используемые в программе
map<string, vector<int>> values;

bool check_symbol(char temp)
{
	return find(sys_symbols.begin(), sys_symbols.end(), temp) != sys_symbols.end();
}

int make_number(istream& in)
{
	char temp = 'a';
	int zn = 0, result = 0;
	while (in >> temp && temp == ' ');

	if (temp == '-' )
	{
		zn = -1;
	}
	else if (temp == '+')
	{
		zn = 1;
	}
	else if(!isdigit(temp)&&!isalpha(temp)&&!check_symbol(temp)) {
		throw 
			invalid_argument("Использовагие недопустимого символа в числе в позиции: " + to_string(in.tellg()) + 
				" значение: " + temp);
	}
	else
	{
		throw
			invalid_argument("Неверный синтаксис записи числа: " + to_string(in.tellg()) +" значение: " + temp);
	}

	int count = 0;
	while (count < 5 && in.get(temp))
	{
		if (isdigit(temp))
		{
			result = result * 10 + (temp - '0');
		}
		else
		{
			if(temp == ' ')
			{
				throw
					invalid_argument("Слишком короткая запись числа " + to_string(zn * result));
			}
			else if (!isdigit(temp) && !isalpha(temp) && !check_symbol(temp)) {
				throw
					invalid_argument("Использовагие недопустимого символа в числе в позиции: " + to_string(in.tellg()) +
						" значение: " + temp);
			}
			else
			{
				throw
					invalid_argument("Неверный синтаксис записи числа: " + to_string(in.tellg()) + " значение: " + temp);
			}
		}
		++count;
	}
	if (count == 5)
	{
		in.get(temp);
		if(isdigit(temp))
			throw
				invalid_argument("Слишком длинная запись числа " + to_string(zn * result));
		in.unget();
	}

	return result * zn;
}

vector<int> make_mas(istream& in)
{
	char temp;
	vector<int> res(size_mas);
	in >> temp;
	if (temp != '(')
	{
		if (!isdigit(temp) && !isalpha(temp) && !check_symbol(temp)) {
			throw
				invalid_argument("Использовагие недопустимого символа в массиве в позиции: " + to_string(in.tellg()) +
					" значение: " + temp);
		}
		else
		{
			throw
				invalid_argument("Не правильная запись массива в позиции: " + to_string(in.tellg()) +
					" значение: " + temp);
		}
	}
	for (int i = 0; i < size_mas && in; ++i)
	{
		try
		{
			res[i] = make_number(in);
		}
		catch (invalid_argument& e)
		{
			cout << e.what() << endl;
			throw
				invalid_argument("Не правильная запись массива в позиции: " + to_string(in.tellg()));
		}

		in >> temp;
		if (i < (size_mas - 1) && temp != ',')
		{
			if (!(isdigit(temp) || temp == '+' || temp == '-'))
			{
				throw
					invalid_argument("Использовагие недопустимого символа в массиве в позиции: " + to_string(in.tellg()) +
						" значение: " + temp);
			}
			else
			{
				throw
					invalid_argument("Не правильная запись массива в позиции, отсутствует запятая: "
						+ to_string(in.tellg()) + " значение: " + temp);
			}
		}
	}

	if (temp != ')')
	{
		if (!(isdigit(temp)||temp == '+' ||temp=='-')) {
			throw
				invalid_argument("Использовагие недопустимого символа в массиве в позиции: " + to_string(in.tellg()) +
					" значение: " + temp);
		}
		else if (temp == ',')
		{
			throw
				invalid_argument("Лишняя запятая в массиве!");
		}
		else if (temp == '+' || temp == '-' || isdigit(temp))
		{
			throw
				invalid_argument("Массив должен состоять из "+to_string(size_mas)+" элеметов!");
		}
		else
		{
			throw
				invalid_argument("Не правильная запись массива в позиции, отсутвует закрывающая скобка: " + to_string(in.tellg()) +
					" значение: " + temp);
		}
	}


	if (!in)
	{
		throw
			invalid_argument("Упс поток закончился во время записи массива ");
	}
	return res;
}

string make_indenfecator(istream& in)
{
	string res;
	char temp;
	in >> temp;

	if (isalpha(temp)) res.push_back(temp);
	else if (!isdigit(temp) && !isalpha(temp) && !check_symbol(temp)) {
		throw
			invalid_argument("Использовагие недопустимого символа в числе в позиции: " + to_string(in.tellg()) +
				" значение: " + temp);
	}
	else
	{
		throw
			invalid_argument("Идентификатор должен начинаться с буквы: "
				+ to_string(in.tellg()) + " значение: " + temp);
	}
	int i = 0;
	for (; i < m && in; ++i)
	{
		in.get(temp);
		if (isalpha(temp) || isdigit(temp)) res.push_back(temp);
		else {
			if (temp != ' ' && !check_symbol(temp))
			{
				throw
					invalid_argument("Не допустимый символ: "
						+ to_string(in.tellg()) + " значение: " + temp);
			}
			else
			{
				break;
			}
		}
	}
	
	if (!in)
	{
		throw
			invalid_argument("Упс поток закончился во время записи идентификатора ");
	}
	if (i == m)
	{
		in.get(temp);
		if (isalpha(temp) || isdigit(temp))
		{
			throw
				invalid_argument("Слишком длинная записть идентификатора: "
					+ to_string(in.tellg()) + " значение: " + temp);
		}
	}

	in.unget();
	return res;
}

vector<int> do_oper(vector<int> a, vector<int> b, char oper)
{
	vector<int> res(size_mas);
	if (oper == '+')
	{
		for (int i = 0; i < size_mas; ++i)
			res[i] = a[i] + b[i];
	}
	else if (oper == '-')
	{
		for (int i = 0; i < size_mas; ++i)
			res[i] = a[i] - b[i];
	}
	else if (oper == '*')
	{
		for (int i = 0; i < size_mas; ++i)
			res[i] = a[i] * b[i];
	}
	else if (oper == '/')
	{
		for (int i = 0; i < size_mas; ++i)
			res[i] = a[i] / b[i];
	}
	else
	{
		throw
			invalid_argument("Неправльный символ операции: " + oper);
	}
	return res;
}

void do_program(istream& in)
{
	int s = 0;
	char ch, oper;
	string result;
	vector<int> val1, val2;
	while (s != 6 && in)
	{
		switch (s)
		{
			case 0:
			{
				try
				{
					result = make_indenfecator(in);
				}
				catch (invalid_argument& e)
				{
					cout << e.what() << endl;
					throw
						invalid_argument("Неправильный синтаксис выражения: " + to_string(in.tellg()));
				}
				s = 1;
				break;
			}
			case 1:
			{
				in >> ch;
				if (ch != ':')
				{
					throw
						invalid_argument("Неправильный синтаксис выражения, отсутствует знак ':' : " + to_string(in.tellg()));
				}
				in >> ch;
				if (ch != '=')
				{
					throw
						invalid_argument("Неправильный синтаксис выражения, отсутствует знак = после ':' : " + to_string(in.tellg()) );
				}
				s = 2;
				break;
			}
			case 2:
			{
				in >> ch;
				if (isalpha(ch))
				{
					in.unget();
					try
					{
						string val1_name = make_indenfecator(in);
						auto pos = values.find(val1_name);
						if (pos == values.end())
						{
							throw
								invalid_argument("Неопределенный идентефекатор в выражении " + to_string(in.tellg())+"\n"+
								"Идентефекатор: "+val1_name);
						}
						val1 = values[val1_name];
					}
					catch (invalid_argument& e)
					{
						cout << e.what() << endl;
						throw
							invalid_argument("Неправильный синтаксис выражения: " + to_string(in.tellg()));
					}
				}
				else if (ch == '(')
				{
					in.unget();
					try
					{
						val1 = make_mas(in);
					}
					catch (invalid_argument& e)
					{
						cout << e.what() << endl;
						throw
							invalid_argument("Неправильный синтаксис выражения: " + to_string(in.tellg()));
					}
				}
				else
				{
					throw
						invalid_argument("Неправильный синтаксис выражения, использован недопустимый символ: " + to_string(in.tellg()));
				}
				s = 3;
				break;
			}
			case 3:
			{
				in >> ch;
				if (ch == ';')
				{
					s = 6;
				}
				else if(ch == '+' || ch == '-' ||
					ch == '*' || ch == '/')
				{
					oper = ch;
					s = 4;
				}
				else
				{
					throw
						invalid_argument("Неправильный синтаксис выражения, использован недопустимый символ: " + to_string(in.tellg())+
						" символ:" + ch);
				}
				break;
			}
			case 4:
			{
				in >> ch;
				if (isalpha(ch))
				{
					in.unget();
					try
					{
						string val2_name = make_indenfecator(in);
						auto pos = values.find(val2_name);
						if (pos == values.end())
						{
							throw
								invalid_argument("Неопределенны идентефекатор в выражении " + to_string(in.tellg()) + "\n" +
									"Идентефекатор: " + val2_name);
						}
						val2 = values[val2_name];
					}
					catch (invalid_argument& e)
					{
						cout << e.what() << endl;
						throw
							invalid_argument("Неправильный синтаксис выражения: " + to_string(in.tellg()));
					}
				}
				else if (ch == '(')
				{
					in.unget();
					try
					{
						val2 = make_mas(in);
					}
					catch (invalid_argument& e)
					{
						cout << e.what() << endl;
						throw
							invalid_argument("Неправильный синтаксис выражения: " + to_string(in.tellg()));
					}
				}
				else
				{
					throw
						invalid_argument("Неправильный синтаксис выражения: " + to_string(in.tellg()));
				}
				s = 5;
				break;
			}
			case 5:
			{
				try
				{
					val1 = do_oper(val1, val2, oper);
				}
				catch (invalid_argument& e)
				{
					cout << e.what() << endl;
					throw
						invalid_argument("Неправильный синтаксис выражения: " + to_string(in.tellg()));
				}
				s = 3;
				break;
			}
			default:
			{
				throw
					runtime_error("Что-то пошло не так!!!");
			}
		}
	}

	if (!in)
	{
		throw
			invalid_argument("Упс поток закончился во время записи идентификатора ");
	}
	else if (s != 6)
	{
		throw
			runtime_error("Что-то пошло не так!!!");
	}

	values[result] = val1;
}

int main()
{
	setlocale(LC_ALL, "ru");
	ifstream in("input.txt");
	bool flag = true;
	cout << in.is_open() << endl;
	char ch;
	in >> ch;
	while (!in.eof())
	{
		in.unget();
		try {
			do_program(in);
		}
		catch (exception& e)
		{
			cout << e.what() << endl;
			flag = false;
			break;
		}

		in >> ch;
	}
	if (flag)
	{
		cout << "Результирующие данные: " << endl;
		for (auto x : values)
		{
			vector<int> mas = x.second;
			cout << x.first << " : ";
			for (int i = 0; i < mas.size(); ++i)
			{
				cout << mas[i] << ' ';
			}
			cout << endl;
		}
	}
	return 0;
}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
