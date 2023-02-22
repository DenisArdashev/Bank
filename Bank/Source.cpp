#include <iostream>
#include <Windows.h>
#include <conio.h>
#include<time.h>
#include<vector>
#include<string>
#include<algorithm>
using namespace  std;

enum Color { Black = 0, Blue, Green, Cyan, Red, Magenta, Brown, LightGray, DarkGray, LightBlue, LightGreen, LightCyan, LightRed, LightMagenta, Yellow, White };

enum Direction { Up = 72, Left = 75, Right = 77, Down = 80, Enter = 13, esc = 27 };
HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);//получает дискриптор активного окна

void SetColor(Color text, Color background)
{
	SetConsoleTextAttribute(hStdOut, (WORD)((background << 4) | text));
}

void SetCursor(int x, int y)
{
	COORD myCoords = { x,y };//инициализируем передаваемыми значениями объект координат
	SetConsoleCursorPosition(hStdOut, myCoords);
}

class Client
{
	string Name;
	int id;
	double many;
	static int percent;
public:
	string setName()
	{
		string name[5] = { "Маша","Вася","Петя","Ира","Коля" };
		return Name = name[rand() % 5];
	}
	int setId()
	{
		return id = 1 + rand() % 1000;
	}
	int setId(int Id)
	{
		id = Id;
	}
	void setMany(double cash)
	{
		many += cash;
	}
	static void setPercent(int pers)
	{
		percent = pers;
	}
	static int getPersent()
	{
		return percent;
	}
	string getName()
	{
		return Name;
	}
	int getId()
	{
		return id;
	}
	double getMany()
	{
		return many;
	}
	Client()
	{
		Name = setName();
		id = setId();
		many = rand() % 1000;
	}
	Client(string name, int Id, double Many)
	{
		Name = name;
		id = Id;
		many = Many;
	}
	void showClient()
	{
		cout << "Имя: " << Name << endl;
		cout << "Id: " << id << endl;
		cout << "Баланс: " << many << endl;
	}
};
int Client::percent = 2;//уточнение области видимости
//к статическим компонентам - к полям и к методам. можно обращаться ДО создания экземпляра класса
//статические методы иногда называют обслуживающими методами. В статических методах НЕЛЬЗЯ обращаться к нестатическим компонента

/*
Описать класс БАНК, который будет работаться с динамическим массивом клиентов.
Каждый клиент представляет собой набор полей - имя, id и денежный балланс.
Реализовать работу используя кольцевую очередь с приоритетом.
Когда клиент попадает в "обработку" ему начисляются прценты. к пример 2%.
Выводится сообщение - "имя клиента начисленны проценты".
1 вариант: После чего клиент отправляется в хвост очереди
2 вариант: запоминаем id обработанных клиентов и при выборе следующего наивысшего приоритета
пропускаем "обработанных" клиентов.

Реализовать методы:
Показа очереди
Добавления элемента в очередь

Изменение балланса пользователя - имитировать растраты и пополнение счёта.Добавляется и отнимается случайное значение.

//ДОДЕЛАТЬ   10 12 45
Удаления элемента очереди на тот случай. если его балланс стал равен 0
Если балланс изменился так, что у объекта должно измениться его место в очереди - запуск сортировки
Начисление роцентов, согласно приоритету - т.е. у кого больший денежный балланс - того "обрабатывают" первым.
*/
class Bank
{
	vector <Client>  Clients;
	int countUsedClients = 0;
	static bool BigMany(Client a, Client b)
	{
		return a.getMany() > b.getMany();
	}
public:
	void showClients()
	{
		for (int i = 0; i < Clients.size(); i++)
		{
			Clients.at(i).showClient();
			cout << "\n\n";
		}
	}

	void SortClients()
	{
		sort(Clients.begin(), Clients.end() - countUsedClients, BigMany);
	}
	void CreatClient(Client unknow)
	{
		Clients.insert(Clients.end() - countUsedClients, unknow);
		SortClients();
	}
	void FlueBudget()
	{
		for (int i = 0; i < Clients.size(); i++)
		{
			Clients.at(i).setMany(-500 + rand() % 1001);
		}
		SortClients();
	}
	void UsedClient()
	{
		Client Copy;
		if (!Clients.empty())
		{
			Copy = Clients.at(0);
			double var = Copy.getMany();
			var *= (Copy.getPersent()) / 100.0;
			Copy.setMany(var);
			Clients.erase(Clients.begin());
			Clients.push_back(Copy);
			countUsedClients++;
		}
	}
	int ChekBalance()
	{
		int poz = 0;
		for (int i = 0; i < Clients.size(); i++)
		{
			if (Clients.at(i).getMany() <= 0)
			{
				
				return i;				
			}
		}
		return -1;
	}
	void deleteClient(int index)
	{
		Clients.erase(Clients.begin() + index);
	}
	bool Compare()
	{
		return countUsedClients < Clients.size();
	}
	void ZeroCount()
	{
		countUsedClients = 0;
	}
};
int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	srand(time(NULL));
	Bank trust;
	int length = 5;
	for (size_t i = 0; i < length; i++)
	{
		//Client obj;
		trust.CreatClient(Client());
	}
	trust.showClients();	
	trust.CreatClient(Client("ddd", 123, 234));
	cout << endl;
	trust.showClients();
	cout << endl;
	for (int i = 0; i < 3; i++)
	{
		cout << "Изменение балансов на счетах" << "\n";
		trust.FlueBudget();
		do
		{
			if (trust.ChekBalance() != -1)
			{				
				trust.deleteClient(trust.ChekBalance());
			}
		} while (trust.ChekBalance() != -1);

		do
		{
			trust.UsedClient();
		} while (trust.Compare());
		trust.ZeroCount();
		cout << "период " << i << endl;
		trust.showClients();
	}
	return 0;
}