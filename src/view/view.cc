#include "view.h"

#include "./ui_view.h"
namespace s21 {
View::View(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::View),
      controller_(),
      memento_(nullptr),
      my_settings("DreamTeam", "3DViewer"),
      kRad(0.017453292519943295474) {
  QSurfaceFormat fmt;
  fmt.setVersion(4, 1), fmt.setProfile(QSurfaceFormat::CoreProfile);
  fmt.setDefaultFormat(fmt);
  ui->setupUi(this);
  ReadSettings();
  SetConnect();
  ui->render->setMouseTracking(true);
}
void View::SetConnect() {
  connect(ui->buttonOpen, SIGNAL(clicked()), this, SLOT(Open()));
  connect(ui->Save, SIGNAL(clicked()), this, SLOT(SaveLoad()));
  connect(ui->Load, SIGNAL(clicked()), this, SLOT(SaveLoad()));
  connect(ui->Picture, SIGNAL(clicked()), this, SLOT(SavePicture()));
  connect(ui->Gif, SIGNAL(clicked()), this, SLOT(SaveGif()));
  connect(ui->radio_central, SIGNAL(clicked()), this, SLOT(SetProjection()));
  connect(ui->radio_parallel, SIGNAL(clicked()), this, SLOT(SetProjection()));
  connect(ui->zoom_plus, SIGNAL(clicked()), this, SLOT(Zoom()));
  connect(ui->zoom_min, SIGNAL(clicked()), this, SLOT(Zoom()));
  connect(ui->zoom, SIGNAL(valueChanged(double)), this,
          SLOT(ZoomValueChanged(double)));
  connect(ui->vertex_shape, SIGNAL(currentIndexChanged(int)), this,
          SLOT(SetVertexShape(int)));
  connect(ui->vertex_size, SIGNAL(valueChanged(int)), this,
          SLOT(SetVertexSize(int)));
  connect(ui->vertex_color, SIGNAL(clicked()), this, SLOT(SetVertexColor()));
  connect(ui->edge_shape, SIGNAL(currentIndexChanged(int)), this,
          SLOT(SetEdgeShape(int)));
  connect(ui->edge_size, SIGNAL(valueChanged(double)), this,
          SLOT(SetEdgeSize(double)));
  connect(ui->edge_color, SIGNAL(clicked()), this, SLOT(SetEdgeColor()));
  connect(ui->background_color, SIGNAL(clicked()), this,
          SLOT(SetBackgroundColor()));
  for (QDoubleSpinBox *b :
       {ui->step_rotate_x, ui->step_rotate_y, ui->step_rotate_z,
        ui->step_trans_x, ui->step_trans_y, ui->step_trans_z, ui->step_zoom}) {
    connect(b, SIGNAL(valueChanged(double)), this, SLOT(ChangeStep(double)));
  }
  // translate
  for (QDoubleSpinBox *b :
       {ui->translate_x, ui->translate_y, ui->translate_z}) {
    connect(b, SIGNAL(valueChanged(double)), this,
            SLOT(TranslateValueChanged(double)));
  }
  for (QPushButton *b :
       {ui->translate_blx, ui->translate_bly, ui->translate_blz,
        ui->translate_brx, ui->translate_bry, ui->translate_brz}) {
    connect(b, SIGNAL(clicked()), SLOT(TranslateButton()));
  }
  // rotate
  for (QDoubleSpinBox *b : {ui->rotate_x, ui->rotate_y, ui->rotate_z}) {
    connect(b, SIGNAL(valueChanged(double)), this,
            SLOT(RotateValueChanged(double)));
    connect(b, SIGNAL(editingFinished()), this, SLOT(EditFinished()));
  }
  for (QPushButton *b : {ui->rotate_blx, ui->rotate_bly, ui->rotate_blz,
                         ui->rotate_brx, ui->rotate_bry, ui->rotate_brz}) {
    connect(b, SIGNAL(clicked()), SLOT(RotateButton()));
  }
}

View::~View() {
  WriteSettings();
  delete memento_;
  ui->render->Clear();
  delete ui;
}

void View::Open() {
  QString name = QFileDialog::getOpenFileName(
      this, "Open file", QCoreApplication::applicationDirPath(), "*.obj", 0,
      QFileDialog::DontUseNativeDialog);
  if (name == "") return;
  CleanMemento();
  CleanViewSetting();
  try {
    controller_.Parsing(name.toStdString());
    Info(name);
    ui->render->Set(controller_.GetData());
    ui->render->update();
  } catch (s21::bad_file) {
    ui->Info->setStyleSheet("color: red;");
    ui->Info->setText(QFileInfo(name).fileName() + " incorrect obj file");
  } catch (s21::not_open) {
    ui->Info->setStyleSheet("color: red;");
    ui->Info->setText(QFileInfo(name).fileName() + " not found");
  }
}
// Clean memento and nulling him
void View::CleanMemento() {
  delete memento_;
  memento_ = nullptr;
}
void View::SaveLoad() {
  QString str = reinterpret_cast<QPushButton *>(sender())->text();
  if (str == "save") {
    Command *c = new SaveCommand(&memento_);
    controller_.Run(c);
  } else if (memento_) {
    CleanViewSetting();
    Command *c = new LoadCommand(&memento_);
    controller_.Run(c);
  }
  ui->render->Load();
  ui->render->update();
}

void View::SavePicture() {
  QFileDialog fileDialog;
  fileDialog.setDefaultSuffix("bmp");
  fileDialog.setAcceptMode(QFileDialog::AcceptSave);
  QString name = fileDialog.getSaveFileName(
      this, QString("сохранить картинку"),
      QCoreApplication::applicationDirPath(), "BMP (*.bmp);; JPEG(*.jpeg)", 0,
      QFileDialog::DontUseNativeDialog);
  if (name != "") {
    if (ui->render->grabFramebuffer().size() == ui->render->size()) {
      QImage img = ui->render->grabFramebuffer();
      img.save(name, NULL, 100);
    } else {
      /*
      GLuint smallFBO, texture;
      glGenFramebuffers(1, &smallFBO);
      glBindFramebuffer(GL_FRAMEBUFFER, smallFBO);
      glGenTextures(1, &texture);
      glBindTexture(GL_TEXTURE_2D, texture);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE,
      NULL); glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

      glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                             GL_TEXTURE_2D, texture, 0);

      if (glCheckFramebufferStatus(GL_FRAMEBUFFER != GL_FRAMEBUFFER_COMPLETE)) {
        qDebug() << "Error \n";
      }

      glBindFramebuffer(GL_FRAMEBUFFER, 0);
      glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE, i.data());
      */
      QImage img = ui->render->grabFramebuffer();
      // img = img.scaled(w, h, Qt::IgnoreAspectRatio,
      // Qt::SmoothTransformation);
      img.save(name, NULL, -1);
    }
  }
}

void View::SaveGif() {
  QFileDialog fileDialog;
  fileDialog.setDefaultSuffix("gif");
  fileDialog.setAcceptMode(QFileDialog::AcceptSave);
  QString name = fileDialog.getSaveFileName(
      this, QString("сохранить gif"), QCoreApplication::applicationDirPath(),
      "*.gif", 0, QFileDialog::DontUseNativeDialog);
  if (name != "") {
    int error = 0;

    GifFileType *gifFile =
        EGifOpenFileName(name.toStdString().data(), false, &error);
    if (!gifFile) return;

    QVector<ColorMapObject *> map(50, nullptr);
    QVector<uchar *> source(50, nullptr);
    if (map.size() != 50 || source.size() != 50) return;

    for (int i = 0; i < 50; ++i) {
      map[i] = GifMakeMapObject(256, NULL);
      if (!map[i]) {
        EGifCloseFile(gifFile, &error);
        FreeColorMap(map);
        return;
      }
    }
    try {
      SavedImage *for_delete = new SavedImage[50];
      QImage img = ui->render->grabFramebuffer();
      GLuint w = img.width(), h = img.height();

      gifFile->SavedImages = for_delete;
      gifFile->ImageCount = 50;
      gifFile->SWidth = w;
      gifFile->SHeight = h;
      gifFile->SColorResolution = 8;
      gifFile->SBackGroundColor = 0;

      // EGifPutScreenDesc(gifFile, w, h, 8, 0, colorMap);
      QWaitCondition wc;
      QMutex mutex;

      for (int i = 0; i < 50; ++i) {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
        // QEventLoop loop;
        // QTimer::singleShot(100, &loop, &QEventLoop::quit);
        // loop.exec();
        mutex.lock();
        wc.wait(&mutex, 100);
        mutex.unlock();

        img = ui->render->grabFramebuffer();
        // img=img.scaled(640, 480, Qt::IgnoreAspectRatio,
        // Qt::SmoothTransformation);
        w = img.width();
        h = img.height();

        ColorMapObject *colorMap = map[i];
        SavedImage *saved_image = &gifFile->SavedImages[i];
        saved_image->ImageDesc.Left = 0;
        saved_image->ImageDesc.Top = 0;
        saved_image->ImageDesc.Width = w;
        saved_image->ImageDesc.Height = h;
        saved_image->ImageDesc.Interlace = false;
        saved_image->ImageDesc.ColorMap = colorMap;
        saved_image->ExtensionBlockCount = 0;
        // saved_image->ExtensionBlocks = nullptr;
        //  saved_image->ExtensionBlocks = new ExtensionBlock;
        //  saved_image->ExtensionBlocks[0].Function = GRAPHICS_EXT_FUNC_CODE;
        //  saved_image->ExtensionBlocks[0].ByteCount = 4;
        //  saved_image->ExtensionBlocks[0].Bytes[1] = 100;
        //  saved_image->ExtensionBlocks[0].Bytes[2] = 0;
        //  saved_image->ExtensionBlocks[0].Bytes[3] = 0;

        // convert to index8 and fill pall
        if (img.format() != QImage::Format_Indexed8) {
          img = img.convertToFormat(QImage::Format_Indexed8);
        }
        QList<QRgb> colorTable = img.colorTable();
        for (int k = 0; k < colorTable.size(); ++k) {
          colorMap->Colors[k].Red = qRed(colorTable[k]);
          colorMap->Colors[k].Green = qGreen(colorTable[k]);
          colorMap->Colors[k].Blue = qBlue(colorTable[k]);
        }

        // copy data from image to SavedImage
        uchar *src = new uchar[img.width() * img.height()];
        source[i] = src;
        size_t size = w * h;
        if (size != img.sizeInBytes()) {
          for (int k = 0; k < h; ++k) {
            memcpy(src + k * w, img.scanLine(k), w);
          }
        } else {
          memcpy(src, img.bits(), size);
        }

        saved_image->RasterBits = src;
        // EGifPutImageDesc(gifFile, 0, 0, w, h, false, colorMap);
        // EGifPutExtensionTrailer(gifFile);
      }
      EGifSpew(gifFile);
      // EGifCloseFile(gifFile, &error);

      // delete colorMap[50] and source[50]
      FreeColorMap(map);
      FreeSourceImage(source);
      delete[] for_delete;
    } catch (...) {
      FreeColorMap(map);
      EGifCloseFile(gifFile, &error);
    }
  }
}

void View::FreeColorMap(QVector<ColorMapObject *> const &m) {
  for (int i = 0; i < m.size(); ++i) {
    if (m[i]) GifFreeMapObject(m[i]);
  }
}
void View::FreeSourceImage(QVector<uchar *> const &s) {
  for (int i = 0; i < s.size(); ++i) {
    delete s[i];
  }
}

void View::Info(QString path) {
  s21::Data &data = controller_.GetData();
  QFileInfo fname(path);
  QString name = fname.fileName();
  ui->Info->setStyleSheet("color: white;");
  ui->Info->setText("Название файла: " + name + "\nКоличество вершин: " +
                    QString::number(data.vertex.size() / 3) +
                    "\nКоличество ребер: " + QString::number(data.edges));
}
// Zoom buttons
void View::Zoom() {
  double step = ui->step_zoom->value();
  double value = ui->zoom->value();
  if (qobject_cast<QPushButton *>(sender())->text() == "+") {
    value += step;
  } else {
    value -= step;
  }
  ui->zoom->setValue(value);
}

void View::ZoomValueChanged(double zoom) {
  double old = ui->render->zoom;
  if (zoom - old) {
    Command *c = new ZoomCommand(zoom / old);
    ui->render->zoom = zoom;
    // Execute command (delete in Model::Run(Command *c))
    controller_.Run(c);
    ui->render->Load();
    ui->render->update();
  }
}

// Translate xyz
void View::TranslateValueChanged(double d) {
  QString str = qobject_cast<QDoubleSpinBox *>(sender())->objectName();
  Command *c = nullptr;
  double len = 0;
  if (str == "translate_x") {
    len = d - ui->render->pos_x;
    ui->render->pos_x = d;
    c = new TranslateXCommand(len);
  } else if (str == "translate_y") {
    len = d - ui->render->pos_y;
    ui->render->pos_y = d;
    c = new TranslateYCommand(len);
  } else if (str == "translate_z") {
    len = d - ui->render->pos_z;
    ui->render->pos_z = d;
    c = new TranslateZCommand(len);
  }
  // Execute command (delete in Model::Run(Command *c))
  controller_.Run(c);
  if (len) {
    ui->render->Load();
  }
  ui->render->update();
}
// Translate buttons
void View::TranslateButton() {
  QString str = reinterpret_cast<QPushButton *>(sender())->objectName();
  QDoubleSpinBox *box = ui->translate_x;
  double s = 0;
  if (str == "translate_blx") {
    box = ui->translate_x;
    s = box->value() - ui->step_trans_x->value();
  } else if (str == "translate_brx") {
    box = ui->translate_x;
    s = box->value() + ui->step_trans_x->value();
  } else if (str == "translate_bly") {
    box = ui->translate_y;
    s = box->value() - ui->step_trans_y->value();
  } else if (str == "translate_bry") {
    box = ui->translate_y;
    s = box->value() + ui->step_trans_y->value();
  } else if (str == "translate_blz") {
    box = ui->translate_z;
    s = box->value() - ui->step_trans_z->value();
  } else if (str == "translate_brz") {
    box = ui->translate_z;
    s = box->value() + ui->step_trans_z->value();
  }
  box->setValue(s);
}

// Set rotate value in QDoubleSpinBox
void View::RotateValueChanged(double d) {
  QString str = qobject_cast<QDoubleSpinBox *>(sender())->objectName();
  Command *c = nullptr;
  QDoubleSpinBox *ptr = ui->rotate_x;
  double angle = 0, *old_ptr = &ui->render->rot_x;
  if (str == "rotate_x") {
    ptr = ui->rotate_x;
    old_ptr = &ui->render->rot_x;
    angle = d - *old_ptr;
    c = new AroundXCommand(angle * kRad);
  } else if (str == "rotate_y") {
    ptr = ui->rotate_y;
    old_ptr = &ui->render->rot_y;
    angle = d - *old_ptr;
    c = new AroundYCommand(angle * kRad);
  } else if (str == "rotate_z") {
    ptr = ui->rotate_z;
    old_ptr = &ui->render->rot_z;
    angle = d - *old_ptr;
    c = new AroundZCommand(angle * kRad);
  }
  if (d >= 360) {
    d = fmod(d, 360);
  }
  if (d <= -360) {
    d = -fmod(d, 360);
  }
  *old_ptr = d;
  ptr->setValue(d);
  // Execute command (delete in Model::Run(Command *c))
  controller_.Run(c);

  if (angle) ui->render->Load();
  ui->render->update();
}

// click button rotate
void View::RotateButton() {
  QString str = reinterpret_cast<QPushButton *>(sender())->objectName();
  QDoubleSpinBox *ptr = ui->rotate_x;
  double value = 0;
  if (str == "rotate_blx") {
    ptr = ui->rotate_x;
    value = ptr->value() - ui->step_rotate_x->value();
  } else if (str == "rotate_brx") {
    ptr = ui->rotate_x;
    value = ptr->value() + ui->step_rotate_x->value();
  } else if (str == "rotate_bly") {
    ptr = ui->rotate_y;
    value = ptr->value() - ui->step_rotate_y->value();
  } else if (str == "rotate_bry") {
    ptr = ui->rotate_y;
    value = ptr->value() + ui->step_rotate_y->value();
  } else if (str == "rotate_blz") {
    ptr = ui->rotate_z;
    value = ptr->value() - ui->step_rotate_z->value();
  } else if (str == "rotate_brz") {
    ptr = ui->rotate_z;
    value = ptr->value() + ui->step_rotate_z->value();
  }
  ptr->setFocus();
  ptr->setValue(value);
}

// clear rotation field after
void View::EditFinished() {
  QString str = reinterpret_cast<QDoubleSpinBox *>(sender())->objectName();
  if (str == "rotate_x") {
    ui->render->rot_x = 0;
    ui->rotate_x->setValue(0.0);
  } else if (str == "rotate_y") {
    ui->render->rot_y = 0;
    ui->rotate_y->setValue(0.0);
  } else if (str == "rotate_z") {
    ui->render->rot_z = 0;
    ui->rotate_z->setValue(0.0);
  }
}

// Mouse control
void View::mousePressEvent(QMouseEvent *event) {
  if (event->button() == Qt::LeftButton || event->button() == Qt::RightButton) {
    start_position = event->pos();
    ui->render->setMouseTracking(true);
  }
}
void View::mouseReleaseEvent(QMouseEvent *event) {
  if (event->button() == Qt::LeftButton || event->button() == Qt::RightButton) {
    ui->render->setMouseTracking(false);
  }
}
void View::mouseMoveEvent(QMouseEvent *event) {
  if (event->buttons() == Qt::LeftButton) {
    QPoint dist = event->pos() - start_position;
    int x = dist.x();
    int y = dist.y();
    int w_s = ui->render->width() / 10;
    int h_s = ui->render->height() / 10;
    int count_step_x = x / w_s;
    if (count_step_x) {
      double step_x = ui->step_trans_x->value();
      ui->translate_x->setValue(ui->translate_x->value() +
                                count_step_x * step_x);
      start_position.setX(start_position.x() + count_step_x * w_s);
    }
    int count_step_y = y / h_s;
    if (count_step_y) {
      double step_y = ui->step_trans_y->value();
      ui->translate_y->setValue(ui->translate_y->value() -
                                count_step_y * step_y);
      start_position.setY(start_position.y() + count_step_y * h_s);
    }
  } else if (event->buttons() == Qt::RightButton) {
    QPoint dist = event->pos() - start_position;
    int x = dist.x();
    int y = dist.y();
    int w_s = ui->render->width() / 16;
    int h_s = ui->render->height() / 16;
    int count_step_x = x / w_s;
    if (count_step_x) {
      ui->rotate_x->setFocus();
      double step_x = ui->step_rotate_x->value();
      ui->rotate_x->setValue(ui->rotate_x->value() + count_step_x * step_x);
      start_position.setX(start_position.x() + count_step_x * w_s);
    }
    int count_step_y = y / h_s;
    if (count_step_y) {
      ui->rotate_y->setFocus();
      double step_y = ui->step_rotate_y->value();
      ui->rotate_y->setValue(ui->rotate_y->value() - count_step_y * step_y);
      start_position.setY(start_position.y() + count_step_y * h_s);
    }
  }
}

// Scrolling
void View::wheelEvent(QWheelEvent *event) {
  QDoubleSpinBox *ptr = ui->zoom;
  if (reinterpret_cast<QWidget *>(QApplication::widgetAt(QCursor::pos()))
          ->objectName() != "render")
    return;
  if (event->angleDelta().y() > 0) {
    ptr->setValue(ptr->value() - ui->step_zoom->value());
  } else {
    ptr->setValue(ptr->value() + ui->step_zoom->value());
  }
}

// Change setting step
void View::ChangeStep(double d) {
  QString str = qobject_cast<QDoubleSpinBox *>(sender())->objectName();
  if (str == "step_zoom") {
    ui->zoom->setSingleStep(ui->step_zoom->value());
  } else if (str == "step_trans_x") {
    ui->translate_x->setSingleStep(d);
  } else if (str == "step_trans_y") {
    ui->translate_y->setSingleStep(d);
  } else if (str == "step_trans_z") {
    ui->translate_z->setSingleStep(d);
  } else if (str == "step_rotate_x") {
    ui->rotate_x->setSingleStep(d);
  } else if (str == "step_rotate_y") {
    ui->rotate_y->setSingleStep(d);
  } else if (str == "step_rotate_z") {
    ui->rotate_z->setSingleStep(d);
  }
}
void View::SetProjection() {
  ui->render->project = ui->radio_central->isChecked();
  ui->render->update();
}
void View::SetVertexShape(int index) {
  ui->render->vertex_shape = index;
  ui->render->update();
}
void View::SetVertexSize(int size) {
  ui->render->vertex_size = size;
  ui->render->update();
}
void View::SetVertexColor() {
  QColor temp = QColorDialog::getColor(ui->render->vertex_color);
  if (temp.isValid()) {
    ui->render->vertex_color = temp;
    ui->render->update();
  }
}

void View::SetEdgeShape(int index) {
  ui->render->edge_shape = index;
  ui->render->update();
}
void View::SetEdgeSize(double size) {
  ui->render->edge_size = size;
  ui->render->update();
}
void View::SetEdgeColor() {
  QColor temp = QColorDialog::getColor(ui->render->edge_color);
  if (temp.isValid()) {
    ui->render->edge_color = temp;
    ui->render->update();
  }
}

void View::SetBackgroundColor() {
  QColor temp = QColorDialog::getColor(ui->render->back_color);
  if (temp.isValid()) {
    ui->render->back_color = temp;
    ui->render->update();
  }
}
// Clean
void View::CleanViewSetting() {
  for (QDoubleSpinBox *b :
       {ui->rotate_x, ui->rotate_y, ui->rotate_z, ui->translate_x,
        ui->translate_y, ui->translate_z}) {
    b->setValue(0);
  }
  ui->zoom->setValue(1.0);
  SetSetting();
}
void View::SetSetting() {
  ui->render->pos_x = ui->render->pos_y = ui->render->pos_z = 0.0;
  ui->render->zoom = 1.0;
}
// Settings
void View::WriteSettings() {
  my_settings.beginGroup("/Settings");
  my_settings.setValue("/vertex_shape", ui->render->vertex_shape);
  my_settings.setValue("/vertex_size", ui->render->vertex_size);
  my_settings.setValue("/vertex_color", ui->render->vertex_color.name());
  my_settings.setValue("/edge_color", ui->render->edge_color.name());
  my_settings.setValue("/back_color", ui->render->back_color.name());
  my_settings.setValue("/edge_width", ui->render->edge_size);
  my_settings.setValue("/edge_pattern", ui->render->edge_shape);
  my_settings.setValue("/proection", ui->radio_central->isChecked());
  my_settings.endGroup();
}

void View::ReadSettings() {
  my_settings.beginGroup("/Settings");
  ui->render->vertex_shape = my_settings.value("/vertex_shape", "0").toInt();
  ui->render->vertex_size = my_settings.value("/vertex_size", "5").toInt();
  ui->render->edge_size = my_settings.value("/edge_width", "1").toDouble();
  ui->render->edge_shape = my_settings.value("/edge_pattern", "0").toInt();
  ui->render->project = my_settings.value("/proection", "0").toInt();
  ui->render->vertex_color.setRgb(
      QColor((my_settings.value("/vertex_color", "#ff0000").toString())).rgb());
  ui->render->edge_color.setRgb(
      QColor((my_settings.value("/edge_color", "#0000ff").toString())).rgb());
  ui->render->back_color.setRgb(
      QColor((my_settings.value("/back_color", "#ffffff").toString())).rgb());

  ui->vertex_shape->setCurrentIndex(ui->render->vertex_shape);
  ui->vertex_size->setValue(ui->render->vertex_size);
  ui->edge_size->setValue(ui->render->edge_size);
  ui->edge_shape->setCurrentIndex(ui->render->edge_shape);
  if (ui->render->project) ui->radio_central->setChecked(true);
  my_settings.endGroup();
}

}  // end namespace s21
