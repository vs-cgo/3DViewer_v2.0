#ifndef _3DVIEWER_v2_0_CONTROLLER_CONTROLLER_H_
#define _3DVIEWER_v2_0_CONTROLLER_CONTROLLER_H_

#include "../model/model.h"

namespace s21 {
/**
 * @brief The Controller class для связывания класса модели и вью
 */
class Controller {
 public:
  Controller();
  ~Controller();
  /**
   * @brief Parsing метод для пути к obj файлу в модель
   * @param[in] str путь к файлу
   */
  void Parsing(const std::string &str);
  /**
   * @brief Run метод для передачи команды в модель для выполнения
   * @param[in] c указатель на базовый класс по которому передается
   * соответственная команда
   */
  void Run(Command *c);

  /**
   * @brief GetData метод для получения структуры данных для визуализации
   * @return структура данных
   */
  Data &GetData();

 private:
  Model model_;
};
}  // end namespace s21
#endif  // _3DVIEWER_v2_0_CONTROLLER_CONTROLLER_H_
