#ifndef _3DVIEWER_v2_0_MODEL_PARS_H_
#define _3DVIEWER_v2_0_MODEL_PARS_H_

#include <cstring>
#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include <vector>

#include "data.h"

namespace s21 {
/**
 * @brief The not_open struct выбрасывается в качестве исключения, если
 * невозможно открыть файл
 */
struct not_open {};
/**
 * @brief The bad_file struct выбрасывается если: в строке нет хотя бы 2 вершин,
 * индекс примитива больше возможной существующей вершины, нет ни одного индекса
 * для полигона, нет ни одной вершины
 */
struct bad_file {};

/**
 * @brief The Pars class для парсинга данных из файла в структуру Data
 */
class Pars {
 public:
  /**
   * @brief Pars c-tor
   */
  Pars() = default;
  /**
   * @brief Parsing метод для парсинга файла
   * @param[in] input путь к obj файлу
   * @param[in] data сслка на структуру data
   */
  void Parsing(const std::string &input, Data &data);

 private:
  void MinMaxInit(std::vector<double> &, double, double, double);
  void MinMax(std::vector<double> &, double, double, double);
  void CountEdges(Data &data);
  void clean(Data &data);
};
}  // namespace s21
#endif  // _3DVIEWER_v2_0_MODEL_PARS_H_
