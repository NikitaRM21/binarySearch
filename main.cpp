/*!
\file
\brief Файл лабораторной работы

Данный файл содержит в себе определения и описания элементов, используемых в лабораторной
*/
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <chrono>
#include <map>

using namespace std;
/*!
    \brief Класс записи из CSV
    \author Никита Романов
    \version 1.0
    \date Январь 2023 года

    Содержит ФИО, название улицы, номер дома, квартиры, дату рождения. Перегружены операторы сравнения.
    Приоритет сравнения: улица, дом, квартира, фио, рождение.
*/
class Entry {
public:
    Entry() {}
    Entry(string fio, string street, int home, int flat, int birth){
        this->street=street;
        this->fio=fio;
        this->home=home;
        this->flat=flat;
        this->birth=birth;
    }

    //приоритет сравнения: улица дом квартира фио рождение
    bool operator>(Entry &second) {
        if (this->fio!=second.fio) {
            return this->fio > second.fio;

        } else if(this->street!=second.street) {
            return this->street > second.street;

        } else if(this->home!=second.home) {
            return this->home > second.home;

        } else if(this->flat!=second.flat) {
            return this->flat > second.flat;

        } else {
            return this->birth > second.birth;
        }
    }

    bool operator<(Entry &second) {
        if (this->fio!=second.fio) {
            return this->fio < second.fio;
        } else if(this->street!=second.street) {
            return this->street < second.street;

        } else if(this->home!=second.home) {
            return this->home < second.home;

        } else if(this->flat!=second.flat) {
            return this->flat < second.flat;
        } else {
            return this->birth < second.birth;
        }
    }

    bool operator>=(Entry &second) {
        if (this->fio!=second.fio) {
            return this->fio >= second.fio;

        } else if(this->street!=second.street) {
            return this->street >= second.street;

        } else if(this->home!=second.home) {
            return this->home >= second.home;

        } else if(this->flat!=second.flat) {
            return this->flat >= second.flat;
        } else {
            return this->birth >= second.birth;
        }
    }

    bool operator<=(Entry &second) {
        if (this->fio!=second.fio) {
            return this->fio <= second.fio;

        } else if(this->street!=second.street) {
            return this->street <= second.street;

        } else if(this->home!=second.home) {
            return this->home <= second.home;

        } else if(this->flat!=second.flat) {
            return this->flat <= second.flat;
        } else {
            return this->birth <= second.birth;
        }
    }

    string fio;
    string street;
    int home;
    int flat;
    int birth;
};

ostream& operator<<(ostream& out, const Entry& entry) {
    return out << entry.fio << "," << entry.street << "," << entry.home << "," << entry.flat << "," << entry.birth << endl;
}

/*!
Собирает из массива "кучу"
\param[a] a Массив данных
\param[k] k номер элемента с которого бежим вниз
\param[n] n Размер массива
\return Ничего не возвращает
*/
template<class T> void createHeap(vector<T>& a, long k, long n){
    long child;
    //будем продолжать спускаться вниз по куче, пока есть элемент у которого потомок больше него
    while (((2*k+1<=n and a[2*k+1]>a[k]) or (2*k+2<=n and a[2*k+2]>a[k]))) {
        child = 2*k+1;
        //выбираем бОльшего ребёнка(заодно проверяем, что элемент не последний, иначе КУДА ТЫ ЕГО УВЕЛИЧИВАТЬ ТО БУДЕШЬ НА ОДИН?)
        if (child < n and a[child]<a[child+1]) {
            ++child;
        }
        //махнём неглядя два элемента и будем смотреть дальше по куче относительно позиици ребёнка (свопать и его потомов тоже)
        if (a[child]>a[k]) {
            swap(a[k],a[child]);
            k = child;
        } else {
            break;
        }
    }
}

/*!
Сортирует методом кучи (пирамиды)
\param[arr] a Массив данных
\param[size] size Размер массива
\return Ничего не возвращает
*/
template<class T>
void heapSort(vector<T> &a,long size) {
    //идем по элементам у которых есть потомки и собираем первичкую кучу
    for (long i = size/2-1; i>=0; --i) {
        createHeap(a,i,size-1);
    }
    for (long j = size; j>=2; --j) {
        swap(a[0],a[j-1]);
        createHeap(a,0,j-2);
    }
}
/*!
Алгоритм бинарного поиска
\param[a] a Массив данных
\param[start] start Стартовая позиция
\param[end] end Конечная позиция
\param[b] b ключ
\return Если нашась возвращает номер записи, иначе -1.
*/
int binarySearch(vector<Entry> &a, int start, int end, string &key)
{
    // Условие завершения: начальный индекс больше конечного индекса
    if (start > end) {
        return -1;
    }

    // Находит средний элемент вектора и использует его для разделения массива на две части.
    const int middle = start + ((end - start) / 2);

    if(a[middle].fio == key) {
        return middle;

    } else if(a[middle].fio > key) {
        return binarySearch(a, start, middle - 1, key);

    }
    return binarySearch(a, middle + 1, end, key);
}

/*!
Алгоритм линейного поиска
\param[a] a Массив данных
\param[start] start Стартовая позиция
\param[size] size Размер массива
\param[b] b ключ
\return Если нашась возвращает номер записи, иначе -1.
*/
int linearSearch(vector<Entry> &a, long start, long size, string &b) {
  for (int i = start; i <=size; i++) {
    if (a[i].fio == b)
      return i;
  }
  return -1;
}

/*!
Парсит данные из файла, сортирует в соответствии с выбранным алгоритмом. Записывает результат в файл /out/имяфайла.
\param[fileName] fileName Имя файла
\param[sortAlgo] sortAlgo Нормер алгоритма
\return Ничего не возвращает
*/
void handleFile(string fileName, int searchAlgo, string key) {
    string openPath = fileName;
    ifstream file(openPath);

    multimap<string, Entry> myMultimap;
    vector<Entry> data;
    string line, field;

    //parce from the file into data
    while ( getline(file,line) )    // get next line in file
    {
        stringstream ss(line);
        int i = 0;
        string fio, street;
        int home, flat, birth;
        while (getline(ss,field,','))
        {
            switch (i) {
            case 0:
                fio = field;
                break;
            case 1:
                street = field;
                break;
            case 2:
                home = stoi(field);
                break;
            case 3:
                flat = stoi(field);
                break;
            case 4:
                birth = stoi(field);
                break;
            default:
                break;
            }
            ++i;
        }
        Entry entry(fio,street,home,flat,birth);
        data.push_back(entry);
        myMultimap.insert(pair<string, Entry>(fio, entry));
    }

    //1-линейные поиск, 2-бинарный в отсортированном, 3-сортировка и бинарный поиск
    if (searchAlgo==1) {
            auto begin = std::chrono::steady_clock::now();
            long i = linearSearch(data, 0, data.size()-1, key);
            while (i != -1) {
                cout << "Element A[" << i << "] = " << data[i] << " found" << endl;
                i = linearSearch (data, i+1, data.size()-1 , key);
            }
            auto end = chrono::steady_clock::now();
            auto elapsed_ms = chrono::duration_cast<chrono::microseconds>(end - begin);
            cout << "File:" << fileName << " Alg:" << searchAlgo << " Time:" << elapsed_ms.count() << " mk\n";
    } else if (searchAlgo==2) {
        auto begin = std::chrono::steady_clock::now();
        long i = binarySearch(data, 0, data.size()-1, key);
        if (i != -1) {
            while (data[i].fio == key) {
                i-=1;
            }
            while (data[i+1].fio == key) {
                cout << "Element A[" << i+1 << "] = " << data[i+1] << " found" << endl;
                ++i;
            }
        }
        auto end = chrono::steady_clock::now();
        auto elapsed_ms = chrono::duration_cast<chrono::microseconds>(end - begin);
        cout << "File:" << fileName << " Alg:" << searchAlgo << " Time:" << elapsed_ms.count() << " mk\n";
    } else if (searchAlgo==3) {
        auto begin = std::chrono::steady_clock::now();
        heapSort(data, data.size());
        long i = binarySearch(data, 0, data.size()-1, key);
        if (i != -1) {
            while (data[i].fio == key) {
                i-=1;
            }
            while (data[i+1].fio == key) {
                cout << "Element A[" << i+1 << "] = " << data[i+1] << " found" << endl;
                ++i;
            }
        }
        auto end = chrono::steady_clock::now();
        auto elapsed_ms = chrono::duration_cast<chrono::microseconds>(end - begin);
        cout << "File:" << fileName << " Alg:" << searchAlgo << " Time:" << elapsed_ms.count() << " mk\n";
    } else if (searchAlgo==4) {
        pair <multimap<string, Entry>::const_iterator, multimap<string, Entry>::const_iterator> p1;
        auto begin = std::chrono::steady_clock::now();
        p1 = myMultimap.equal_range( key );
        auto end = chrono::steady_clock::now();
        auto elapsed_ms = chrono::duration_cast<chrono::microseconds>(end - begin);
        int quantity = 0;
        while (p1.first!=p1.second) {
          quantity++;
          p1.first++;
        }
        std::cout << "Found: " << quantity << " elements" << " Time:" << elapsed_ms.count() << " mk\n";
    }
}

int main() {
//    //1-линейные поиск, 2-бинарный в отсортированном, 3-сортировка и бинарный поиск, 4-multimap
  handleFile("tests/test200.txt", 1, "qzitqhj epfakgvmh aawyfh");
  handleFile("tests/test500.txt", 1, "qzitqhj epfakgvmh aawyfh");
  handleFile("tests/test1000.txt", 1, "qzitqhj epfakgvmh aawyfh");
  handleFile("tests/test8000.txt", 1, "qzitqhj epfakgvmh aawyfh");
  handleFile("tests/test30000.txt", 1, "qzitqhj epfakgvmh aawyfh");
  handleFile("tests/test60000.txt", 1, "qzitqhj epfakgvmh aawyfh");
  handleFile("tests/test120000.txt", 1, "qzitqhj epfakgvmh aawyfh");
  cout << "---------------------" << endl;
  handleFile("sorted/test200.txt", 2, "qzitqhj epfakgvmh aawyfh");
  handleFile("sorted/test500.txt", 2, "qzitqhj epfakgvmh aawyfh");
  handleFile("sorted/test1000.txt", 2, "qzitqhj epfakgvmh aawyfh");
  handleFile("sorted/test8000.txt", 2, "qzitqhj epfakgvmh aawyfh");
  handleFile("sorted/test30000.txt", 2, "qzitqhj epfakgvmh aawyfh");
  handleFile("sorted/test60000.txt", 2, "qzitqhj epfakgvmh aawyfh");
  handleFile("sorted/test120000.txt", 2, "qzitqhj epfakgvmh aawyfh");
//  handleFile("same.txt", 2, "hauvguxzbrh aqensal zlicvotpw"); //пример, показывающий, что находит все одинаковые ключи
  cout << "---------------------" << endl;
  handleFile("tests/test200.txt", 3, "qzitqhj epfakgvmh aawyfh");
  handleFile("tests/test500.txt", 3, "qzitqhj epfakgvmh aawyfh");
  handleFile("tests/test1000.txt", 3, "qzitqhj epfakgvmh aawyfh");
  handleFile("tests/test8000.txt", 3, "qzitqhj epfakgvmh aawyfh");
  handleFile("tests/test30000.txt", 3, "qzitqhj epfakgvmh aawyfh");
  handleFile("tests/test60000.txt", 3, "qzitqhj epfakgvmh aawyfh");
  handleFile("tests/test120000.txt", 3, "qzitqhj epfakgvmh aawyfh");
  cout << "---------------------" << endl;
//  handleFile("same.txt", 4, "hauvguxzbrh aqensal zlicvotpw"); //пример, показывающий, что находит все одинаковые ключи
  handleFile("tests/test200.txt", 4, "qzitqhj epfakgvmh aawyfh");
  handleFile("tests/test500.txt", 4, "qzitqhj epfakgvmh aawyfh");
  handleFile("tests/test1000.txt", 4, "qzitqhj epfakgvmh aawyfh");
  handleFile("tests/test8000.txt", 4, "qzitqhj epfakgvmh aawyfh");
  handleFile("tests/test30000.txt", 4, "qzitqhj epfakgvmh aawyfh");
  handleFile("tests/test60000.txt", 4, "qzitqhj epfakgvmh aawyfh");
  handleFile("tests/test120000.txt", 4, "qzitqhj epfakgvmh aawyfh");
  return 0;

}
