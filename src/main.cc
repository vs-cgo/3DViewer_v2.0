#include <QApplication>

#include "view/view.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  // QSurfaceFormat fmt;
  // fmt.setVersion(3, 3);
  // fmt.setProfile(QSurfaceFormat::CoreProfile);
  // QSurfaceFormat::setDefaultFormat(fmt);
  setlocale(LC_NUMERIC, "en_US.UTF-8");
  s21::View v;
  v.setWindowTitle("3DViewer v2.0");
  QPixmap pix(":/img/pic.png");
  QPalette pal;
  pal.setBrush(v.backgroundRole(), QBrush(pix));
  v.setPalette(pal);
  v.setAutoFillBackground(true);
  v.resize(1000, 580);
  v.show();
  return a.exec();
}
