#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#define GL_SILENCE_DEPRECATION

#ifdef __linux__
#define GL_GLEXT_PROTOTYPES
#endif

//#include <QMatrix4x4>
//#include "glm/glm.hpp"
//#include "glm/gtc/matrix_transform.hpp"
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QOpenGLWidget>
#include <QSurface>
#include <vector>

#include "../model/data.h"

namespace s21 {
/**
 * @brief The Render class для работы с опенгл
 */
class Render : public QOpenGLWidget {
  Q_OBJECT
 public:
  Render(QWidget *parent);
  // protected:
  /**
   * @brief initializeGL Эта функция должна настроить все необходимые ресурсы
   * OpenGL.
   */
  virtual void initializeGL() override;

  /**
   * @brief resizeGL Эта виртуальная функция вызывается всякий раз, когда
   * изменяется размер виджета
   * @param[in] w ширина
   * @param[in] h высота
   */
  virtual void resizeGL(int w, int h) override;

  /**
   * @brief paintGL функция вызывается всякий раз, когда виджет необходимо
   * отрисовать
   */
  virtual void paintGL() override;

  /**
   * @brief Set первичная загрузка данных в контекст
   * @param[in] data ссылка на данные из модели
   */
  void Set(Data &data);

  /**
   * @brief Clear jxbcnrf VAO, VBO, EBO
   */
  void Clear();

  /**
   * @brief Load загрузка данных из модели в контекст
   */
  void Load();

 public:
  QColor vertex_color; /**< цвет вершин*/
  QColor edge_color;   /**< цвет ребер*/
  QColor back_color;   /**< цвет фона*/
  int vertex_size;     /**< размер вершин*/
  int vertex_shape;    /**< форма вершин*/
  int edge_shape; /**< форма ребер(сплошная, пунктирная)*/
  int project; /**< проекция (ортогональная центральная)*/
  double edge_size; /**< размер ребер*/
  double rot_x;     /**< поворот по оси x*/
  double rot_y;     /**< поворот по оси y*/
  double rot_z;     /**< поворот по оси z*/
  double pos_x;     /**< смещение оси x*/
  double pos_y;     /**< смещение оси y*/
  double pos_z;     /**< смещение оси z*/
  double zoom;      /**< коофицент масштабирования*/

 private:
  void PassColorInShader(int i = 0);
  void Projection(int i = 0);
  Data *data_;
  GLuint shaderProgramP, shaderProgramL, VBO, VAO, EBO;
};
}  // end namespace s21
#endif  // OPENGLWIDGET_H
