#ifndef _3DVIEWER_v2_0_VIEW_MEMENTO_MEMENTO_H_
#define _3DVIEWER_v2_0_VIEW_MEMENTO_MEMENTO_H_

#include "../../model/data.h"

namespace s21 {
/**
 * @brief The Memento class для возможности сохранения/загрузки состояния модели
 */
class Memento {
 public:
  /**
   * @brief Memento c-tor
   * @param[in] data ссылка на структуру data
   */
  explicit Memento(Data &data);

  /**
   * @brief SetState сохранение данных из data в memento
   * @param[in] data ссылка на структуру data
   */
  void SetState(Data const &data);

  /**
   * @brief GetState загрузка данных из memento в data
   * @return
   */
  Data GetState();

 private:
  Data state_;
};
}  // end namespace s21
#endif  //_3DVIEWER_v2_0_VIEW_MEMENTO_MEMENTO_H_
