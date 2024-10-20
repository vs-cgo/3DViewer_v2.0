#ifndef VIEW_H
#define VIEW_H

#include <locale.h>

#include <QColorDialog>
#include <QFileDialog>
#include <QMainWindow>
#include <QMouseEvent>
#include <QMutex>
#include <QSettings>
#include <QTimer>
#include <QWaitCondition>
#include <cmath>

#include "../controller/controller.h"
#include "../gif_lib/gif_lib.h"
#include "../gif_lib/gif_lib_private.h"
#include "memento/memento.h"
#include "render.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class View;
}
QT_END_NAMESPACE

namespace s21 {
/**
 * @brief The View class для настройки и управлени отображаемой модели
 */
class View : public QMainWindow {
  Q_OBJECT
  QSettings my_settings;

 public:
  View(QWidget *parent = nullptr);
  ~View();

 private:
  void Info(QString);
  void SetConnect();
  void WriteSettings();
  void ReadSettings();
  // может быть немного сменить название...
  void CleanViewSetting();
  void CleanMemento();
  void SetSetting();
  // free for gif_save
  void FreeColorMap(QVector<ColorMapObject *> const &m);
  void FreeSourceImage(QVector<uchar *> const &s);

 private slots:
  void Open();
  void SaveLoad();
  void SavePicture();
  void SaveGif();
  void Zoom();
  void ZoomValueChanged(double);
  void ChangeStep(double);
  void TranslateValueChanged(double);
  void TranslateButton();
  void RotateValueChanged(double);
  void RotateButton();
  void EditFinished();

  void mousePressEvent(QMouseEvent *event);
  void mouseReleaseEvent(QMouseEvent *event);
  void mouseMoveEvent(QMouseEvent *event);
  void wheelEvent(QWheelEvent *event);
  void SetProjection();
  void SetVertexShape(int);
  void SetVertexSize(int);
  void SetVertexColor();
  void SetEdgeShape(int);
  void SetEdgeSize(double);
  void SetEdgeColor();
  void SetBackgroundColor();

 private:
  Ui::View *ui;
  Controller controller_;
  Memento *memento_;
  QPoint start_position;
  const double kRad;
};
}  // namespace s21
#endif  // VIEW_H
