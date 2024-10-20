#include "pars.h"

namespace s21 {
void Pars::Parsing(const std::string &input, Data &data) {
  std::ifstream is(input);
  if (!is.is_open()) {
    throw not_open();
  }

  clean(data);
  std::vector<double> &mm = data.minmax;
  char temp[256] = {0};
  for (; !is.getline(temp, 255).eof();) {
    if (temp[0] == 'v' && temp[1] == ' ') {
      double x = 0, y = 0, z = 0;
      // если в строке нет хотя бы 2 чисел
      if (sscanf(temp, "v%lf %lf %lf", &x, &y, &z) < 2) throw bad_file();

      data.vertex.emplace_back(x);
      data.vertex.emplace_back(y);
      data.vertex.emplace_back(z);
      data.minmax.empty() ? MinMaxInit(mm, x, y, z) : MinMax(mm, x, y, z);
    } else if (temp[0] == 'f' && temp[1] == ' ') {
      int first = -1, count_vertex = data.vertex.size() / 3;
      for (char *ptr = temp + 2; *ptr;) {
        int value = strtol(ptr, &ptr, 10);
        // если ошибка считывания или номер вершины больше количества вершин
        if (value == 0 || abs(value) > count_vertex) throw bad_file();

        value = value > 0 ? (value - 1) : (count_vertex + value);
        data.polygon.emplace_back(value);
        first < 0 ? (first = value) : data.polygon.emplace_back(value);
        ptr += strcspn(ptr, " ");
        ptr += strspn(ptr, " \f\n\r\t\v");
      }
      // если в строке не было ни одной вершины (f \n)
      if (first == -1) throw bad_file();
      data.polygon.emplace_back(first);
    }
  }
  is.close();
  if (data.vertex.empty()) throw bad_file();  // если не было ни одной вершины
  CountEdges(data);                           // x2  time for open file
}

// Counting unique edges
void Pars::CountEdges(Data &data) {
  std::vector<int> const &polygon = data.polygon;
  std::set<long int> temp;
  for (size_t s = 0, e = polygon.size(); s < e; ++s) {
    int x = polygon[s++];
    int y = polygon[s];
    long int edge = x > y ? x : y;
    edge <<= 32;
    edge += x > y ? y : x;
    temp.insert(edge);
  }
  data.edges = temp.size();
}

void Pars::MinMaxInit(std::vector<double> &mm, double x, double y, double z) {
  mm.emplace_back(x);
  mm.emplace_back(x);
  mm.emplace_back(y);
  mm.emplace_back(y);
  mm.emplace_back(z);
  mm.emplace_back(z);
}

void Pars::MinMax(std::vector<double> &mm, double x, double y, double z) {
  if (mm[0] > x) mm[0] = x;
  if (mm[1] < x) mm[1] = x;
  if (mm[2] > y) mm[2] = y;
  if (mm[3] < y) mm[3] = y;
  if (mm[4] > z) mm[4] = z;
  if (mm[5] < z) mm[5] = z;
}

void Pars::clean(Data &data) {
  data.vertex.clear();
  data.polygon.clear();
  data.minmax.clear();
  data.edges = 0;
}

}  // end namespace s21
