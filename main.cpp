/*
Калугин К. А. М8О-207Б-19
Создать приложение, которое будет считывать из стандартного ввода данные фигур, согласно варианту задания, выводить их характеристики на экран и записывать в файл. Фигуры могут задаваться как своими вершинами, так и другими характеристиками (например, координата центра, количество точек и радиус).
Программа должна:
1.   	Осуществлять ввод из стандартного ввода данных фигур, согласно варианту задания;
2.   	Программа должна создавать классы, соответствующие введенным данных фигур;
3.   	Программа должна содержать внутренний буфер, в который помещаются фигуры. Для создания буфера допускается использовать стандартные контейнеры STL. Размер буфера задается параметром командной строки. Например, для буфера размером 10 фигур: oop_exercise_08 10
4.   	При накоплении буфера они должны запускаться на асинхронную обработку, после чего буфер должен очищаться;
5.   	Обработка должна производиться в отдельном потоке;
6.   	Реализовать два обработчика, которые должны обрабатывать данные буфера:
a.   	Вывод информации о фигурах в буфере на экран;
b.   	Вывод информации о фигурах в буфере в файл. Для каждого буфера должен создаваться файл с уникальным именем.
7.   	Оба обработчика должны обрабатывать каждый введенный буфер. Т.е. после каждого заполнения буфера его содержимое должно выводиться как на экран, так и в файл.
8.   	Обработчики должны быть реализованы в виде лямбда-функций и должны хранится в специальном массиве обработчиков. Откуда и должны последовательно вызываться в потоке – обработчике.
9.   	В программе должно быть ровно два потока (thread). Один основной (main) и второй для обработчиков;
10.	В программе должен явно прослеживаться шаблон Publish-Subscribe. Каждый обработчик должен быть реализован как отдельный подписчик.
11.	Реализовать в основном потоке (main) ожидание обработки буфера в потоке-обработчике. Т.е. после отправки буфера на обработку основной поток должен ждать, пока поток обработчик выведет данные на экран и запишет в файл.
*/
// Вариант - пятиугольник, шестиугольник, восьмиугольник

#include <iostream>
#include <memory>
#include <windows.h>
#include <algorithm>
#include <vector>
#include <math.h>
#include <string>
#include <iterator>
#include <mutex>
#include <queue>
#include <thread>
#include <functional>
#include <fstream>

using namespace std;
const double PI = 3.141592653589793238462643383279502884197169399375105820974944;

const int Five_Id = 1;
const int Six_Id = 2;
const int Eight_Id = 3;
ofstream file;

int nname = 0;

template<class T>
class Q {//класс, выполняющий функцию очереди

	public: 
		queue<T> messages;
		mutex m;

	void Add(T & mes) {
		m.lock();
		messages.push(mes);
		m.unlock();
	}

	bool Empty() {
		m.lock();
		bool a = messages.empty();
		m.unlock();
		return a;
	}

	T Front() {
		m.lock();
		T a = messages.front();
		m.unlock();
		return a;
	}

	void Pop() {
		m.lock();
		messages.pop();
		m.unlock();
	}

};



class Figure {//создание класса фигур
    public:
        virtual void points () {}
		virtual void write() {}
};


template <typename T>
class Five: public Figure {//создание класса-наследника для пятиугольников
	public:
		using type = T;
		pair<T, T> O;
		T R;
        virtual void points () {//функция вывода вершин фигуры
            cout << "----------\n"<< "A = (" << O.first << "," << O.second + R << ")\n" << "B = (" 
                << O.first + R * cos (28.0 / 180.0 * PI) << "," << O.second + R * sin (28.0  / 180.0 * PI) 
                << ")\n" << "C = (" << O.first + R * cos (- 44.0  / 180.0 * PI) << "," 
                << O.second + R * sin (- 44.0  / 180.0 * PI) << ")\n" << "D = (" 
                << O.first + R * cos (- 116.0  / 180.0 * PI) << "," << O.second + R * sin (- 116.0  / 180.0 * PI) 
                << ")\n" << "E = (" << O.first + R * cos (- 188.0  / 180.0 * PI) 
                << "," << O.second + R * sin (- 188.0  / 180.0 * PI) << ")\n" << "----------\n";
        }

        virtual void write () {//функция записи в файл
            file <<"Pentagon\n" << "----------\n"<< "A = (" << O.first << "," << O.second + R << ")\n" << "B = (" 
                 << O.first + R * cos (28.0 / 180.0 * PI) << "," << O.second + R * sin (28.0  / 180.0 * PI) 
                 << ")\n" << "C = (" << O.first + R * cos (- 44.0  / 180.0 * PI) << "," 
                 << O.second + R * sin (- 44.0  / 180.0 * PI) << ")\n" << "D = (" 
                 << O.first + R * cos (- 116.0  / 180.0 * PI) << "," << O.second + R * sin (- 116.0  / 180.0 * PI) 
                 << ")\n" << "E = (" << O.first + R * cos (- 188.0  / 180.0 * PI) 
                 << "," << O.second + R * sin (- 188.0  / 180.0 * PI) << ")\n" << "----------\n";
        }

	    Five () {};//конструктор класса
	    Five (T x, T y, T r): O(x, y), R(r) {};//конструктор класса
};


template <typename T>
class Six: public Figure {//создание класса-наследника для шестиугольников
    public:
        using type = T;
		pair<T, T> O;
		T R;
        virtual void points () {//функция вывода вершин фигуры
            cout << "----------\n" << "A = (" << O.first << "," << O.second + R << ")\n" << "B = (" 
                 << O.first + R * cos (PI / 6.0) << "," << O.second + R * sin (PI / 6.0) 
                 << ")\n" << "C = (" << O.first + R * cos (- PI / 6.0) << "," 
                 << O.second + R * sin (- PI / 6.0) << ")\n" << "D = (" << O.first << "," 
                 << O.second - R << ")\n" << "E = (" << O.first + R * cos (- 5.0 * PI / 6.0) 
                 << "," << O.second + R * sin (- 5.0 * PI / 6.0) << ")\n" << "F = (" 
                 << O.first + R * cos (- 7.0 * PI / 6.0) << "," 
                 << O.second + R * sin (- 7.0 * PI / 6.0) << ")\n" << "----------\n";
        }
        virtual void write () {//функция записи в файл
            file << "Gexagon\n" << "----------\n" << "A = (" << O.first << "," << O.second + R << ")\n" << "B = (" 
                 << O.first + R * cos (PI / 6.0) << "," << O.second + R * sin (PI / 6.0) 
                 << ")\n" << "C = (" << O.first + R * cos (- PI / 6.0) << "," 
                 << O.second + R * sin (- PI / 6.0) << ")\n" << "D = (" << O.first << "," 
                 << O.second - R << ")\n" << "E = (" << O.first + R * cos (- 5.0 * PI / 6.0) 
                 << "," << O.second + R * sin (- 5.0 * PI / 6.0) << ")\n" << "F = (" 
                 << O.first + R * cos (- 7.0 * PI / 6.0) << "," 
                 << O.second + R * sin (- 7.0 * PI / 6.0) << ")\n" << "----------\n";
        }
        Six () {};//конструктор класса
	    Six (T x, T y, T r): O(x, y), R(r) {};//конструктор класса
};

template <typename T>
class Eight: public Figure {//создание класса-наследника для восьмиугольник
    public:
        using type = T;
		pair<T, T> O;
		T R;
        virtual void points () {//функция вывода вершин фигуры
            cout << "----------\n" << "A = (" << O.first << "," << O.second + R << ")\n" << "B = (" 
                 << O.first + R * cos (PI / 4) << "," << O.second + R * sin (PI / 4) 
                 << ")\n" << "C = (" << O.first + R << "," << O.second << ")\n" 
                 << "D = (" << O.first + R * cos (- PI / 4) << "," << O.second + R * sin (- PI / 4) 
                 << ")\n" << "E = (" << O.first << "," << O.second - R << ")\n" 
                 << "F = (" << O.first + R * cos (- 3 * PI / 4) << "," << O.second + R * sin (- 3 * PI / 4) 
                 << ")\n" << "G = (" << O.first - R << "," << O.second << ")\n" << "H = (" 
                 << O.first + R * cos (- 5 * PI / 4) << "," << O.second + R * sin (- 5 * PI / 4) << ") \n" << "----------\n";
        }
        virtual void write () {//функция записи в файл
            file <<"Octogon\n" << "----------\n" << "A = (" << O.first << "," << O.second + R << ")\n" << "B = (" 
                 << O.first + R * cos (PI / 4) << "," << O.second + R * sin (PI / 4) 
                 << ")\n" << "C = (" << O.first + R << "," << O.second << ")\n" 
                 << "D = (" << O.first + R * cos (- PI / 4) << "," << O.second + R * sin (- PI / 4) 
                 << ")\n" << "E = (" << O.first << "," << O.second - R << ")\n" 
                 << "F = (" << O.first + R * cos (- 3 * PI / 4) << "," << O.second + R * sin (- 3 * PI / 4) 
                 << ")\n" << "G = (" << O.first - R << "," << O.second << ")\n" << "H = (" 
                 << O.first + R * cos (- 5 * PI / 4) << "," << O.second + R * sin (- 5 * PI / 4) << ") \n" << "----------\n";
        }
        Eight () {};//конструктор класса
	    Eight (T x, T y, T r): O(x, y), R(r) {};//конструктор класса
};

template<class T, class FIGURE>
class Factory;

template <class T>
class Factory <T, Five <T>> {//класс Factory для создания пятиугольников
    public:
        static shared_ptr <Figure> Create () {
            T x;
            T y;
            T r;
            cout << "Enter center coordinates and radius: ";
            cin >> x; 
            cin >> y;
            cin >> r;
            Five <T>* ret = new Five <T> (x, y, r);
		    return shared_ptr <Figure> (ret);
        }
};

template <class T>
class Factory <T, Six <T>> {//класс Factory для создания шестиугольников
    public:
        static shared_ptr <Figure> Create () {
            T x;
            T y;
            T r;
            cout << "Enter center coordinates and radius: ";
            cin >> x; 
            cin >> y;
            cin >> r;
            Six <T>* ret = new Six <T> (x, y, r);
		    return shared_ptr <Figure> (ret);
        }
};

template <class T>
class Factory <T, Eight <T>> {//класс Factory для создания восьмиугольников
    public:
        static shared_ptr <Figure> Create () {
            T x;
            T y;
            T r;
            cout << "Enter center coordinates and radius: ";
            cin >> x; 
            cin >> y;
            cin >> r;
            Eight <T>* ret = new Eight <T> (x, y, r);
		    return shared_ptr <Figure> (ret);
        }
};

Q <shared_ptr <Figure>> eueu;//объявление объекта класса очереди Q

void threadFunction() {//функция для потока №2

	function <void(shared_ptr<Figure> fig)> Print_fig = [](shared_ptr<Figure> fig) {
		fig->points();
	};

	function<void(shared_ptr<Figure> fig)> Write_fig = [](shared_ptr<Figure> fig) {
		fig->write();
	};

	vector<function<void(shared_ptr<Figure> fig)>> functions({Print_fig, Write_fig});

	while(1) {
		if (!eueu.Empty()) {

			if (eueu.Front() == NULL) {
				break;
			}
            nname ++;
            string name = (to_string (nname)).append (".txt");



            file.open(name.c_str());
			while (!eueu.Empty()) {
				shared_ptr<Figure> el = eueu.Front();
				eueu.Pop();
				for (auto func : functions) {
					func(el);
				}
			}
			file.close ();
		}
	}
} 



int main(int argc, char** argv) {

    thread thread_2(threadFunction);//запуск второго потока
	int size = stoi(string(argv[1]));
    int vsize = 0;

	if (size <= 0) {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, (WORD) ((0 << 4) | 4));
        cout << "ERROR" << endl << "Incorrect buffer length." << endl; 
        SetConsoleTextAttribute(hConsole, (WORD) ((0 << 4) | 7));
		return -1;
	}

    vector < shared_ptr <Figure> > vault;
    int id;
    while (1) {//Ввод фигур
        while (vsize < size) {
            
            cout << "Choose figure type: 1. Pentagon.\n";
            cout << "                    2. Hexagon.\n";
            cout << "                    3. Octogon\n";

            cin >> id;

            if ((id != 1) && (id != 2) && (id != 3)) {
                cout << "ERROR\nIncorrect number.\n";
                break;
            }

            shared_ptr <Figure> nf;

            if (id == Five_Id) {
                nf = Factory<double, Five <double>>::Create ();
            } else if (id == Six_Id) {
                nf = Factory<double, Six <double>>::Create ();
            } else if (id == Eight_Id) {
                nf = Factory<double, Eight <double>>::Create (); 
            }

            vault.push_back (nf);
            vsize ++;
        }

        for (int i = 0; i < vault.size(); i++) {
				eueu.Add(vault[i]);
			}

        while (!eueu.Empty()) {}

		vault.clear();
        vsize = 0;
        int end;
        cin >> end;
        
        if (!end) {        
            break;
        }
    }

    shared_ptr<Figure> null = NULL;//завершение дополнительного потока
	eueu.Add(null);
    thread_2.join();
    return 0;
}