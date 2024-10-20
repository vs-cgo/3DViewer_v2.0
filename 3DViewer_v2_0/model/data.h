#ifndef _3DVIEWER_v2_0_MODEL_DATA_H_
#define _3DVIEWER_v2_0_MODEL_DATA_H_

#include <vector>

namespace s21 {
/**
 * @brief The Data struct для хранения координат вершин, примитивов, количество
 * ребер и минимальные и максимальные координаты по каждой из оси
 */
struct Data {
  std::vector<double> vertex{}; /**< для хранения координат вершин */
  std::vector<int> polygon{}; /**< для хранения примитивов */
  std::vector<double> minmax{}; /**< для хранения минимальной и максимальной
                                   точки по каждой из оси */
  size_t edges{}; /**< для хранения количества ребер */
};
}  // namespace s21
#endif  // _3DVIEWER_v2_0_MODEL_DATA_H_
