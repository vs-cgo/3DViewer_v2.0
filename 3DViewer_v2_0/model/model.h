#ifndef _3DVIEWER_v2_0_MODEL_MODEL_H_
#define _3DVIEWER_v2_0_MODEL_MODEL_H_

#include <iostream>
#include <string>
#include <vector>

#include "command/command.h"
#include "data.h"
#include "pars.h"

namespace s21 {
const int kSize = 1024;

/**
 * @brief The Model class фасад для управления Data и Pars(с возможным
 * расширением)
 */
class Model {
 public:
  using it_i =
      std::vector<int>::iterator; /**< алиас для std::vector<int>::iterator */
  using it_d =
      std::vector<double>::iterator; /**< алиас для
                                        std::vector<double>::iterator */
  /**
   * @brief Model C-tor
   */
  Model();
  /**
   * @brief ~Model D-tor
   */
  ~Model();

  /**
   * @brief Parsing метод для парсинга obj файла и получения данных
   * @param[in] input строка содержащая путь к файлу
   */
  void Parsing(const std::string &input);

  /**
   * @brief Run метод для выполнения переданной команды
   * @param[in] c указатель на базовый класс команды
   */
  void Run(Command *c);

  /**
   * @brief Clean очищения структуры Data
   */
  // void Clean();

  /**
   * @brief GetData метод для получения данных для загрузки в видеокарту
   * @return ссылка на структуру Data
   */
  Data &GetData();

 private:
  Data data_;
  Pars pars_;
};
}  // end namespace s21
#endif  // _3DVIEWER_v2_0_MODEL_MODEL_H_
