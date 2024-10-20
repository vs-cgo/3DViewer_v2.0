#include "command.h"

namespace s21 {
// ZoomCommand
ZoomCommand::ZoomCommand(double z) : value_(z){};
ZoomCommand::~ZoomCommand(){};
void ZoomCommand::execute(Data &data) {
  if (!value_) return;
  using it = std::vector<double>::iterator;
  for (it s = data.vertex.begin(), e = data.vertex.end(); s != e; ++s) {
    *s *= value_;
  }
}

// AroundXCommand
AroundXCommand::AroundXCommand(double a) : angle_(a) {}
AroundXCommand::~AroundXCommand() {}
void AroundXCommand::execute(Data &data) {
  if (!angle_) return;

  double s = sin(angle_);
  double c = cos(angle_);
  double ms = -s;
  std::vector<double> &vertex = data.vertex;
  for (size_t i = 0, end = vertex.size(); i < end; i += 3) {
    double y = vertex[i + 1];
    double z = vertex[i + 2];
    vertex[i + 1] = y * c + z * ms;
    vertex[i + 2] = y * s + z * c;
  }
}

// AroundYCommand
AroundYCommand::AroundYCommand(double a) : angle_(a) {}
AroundYCommand::~AroundYCommand() {}
void AroundYCommand::execute(Data &data) {
  if (!angle_) return;

  double s = sin(angle_);
  double c = cos(angle_);
  double ms = -s;
  std::vector<double> &vertex = data.vertex;
  for (size_t i = 0, end = vertex.size(); i < end; i += 3) {
    double x = vertex[i];
    double z = vertex[i + 2];
    vertex[i] = x * c + z * s;
    vertex[i + 2] = x * ms + z * c;
  }
}

// AroundZCommand
AroundZCommand::AroundZCommand(double a) : angle_(a) {}
AroundZCommand::~AroundZCommand() {}
void AroundZCommand::execute(Data &data) {
  if (!angle_) return;

  double s = sin(angle_);
  double c = cos(angle_);
  double ms = -s;
  std::vector<double> &vertex = data.vertex;
  for (size_t i = 0, end = vertex.size(); i < end; i += 3) {
    double x = vertex[i];
    double y = vertex[i + 1];
    vertex[i] = x * c + y * ms;
    vertex[i + 1] = x * s + y * c;
  }
}

// TranslateXCommand
TranslateXCommand::TranslateXCommand(double v) : value_(v) {}
TranslateXCommand::~TranslateXCommand(){};
void TranslateXCommand::execute(Data &data) {
  if (!value_) return;
  std::vector<double> &vertex = data.vertex;
  for (size_t i = 0, end = vertex.size(); i < end; i += 3) {
    vertex[i] += value_;
  }
}

// TranslateYCommand
TranslateYCommand::TranslateYCommand(double v) : value_(v) {}
TranslateYCommand::~TranslateYCommand(){};
void TranslateYCommand::execute(Data &data) {
  if (!value_) return;
  std::vector<double> &vertex = data.vertex;
  for (size_t i = 1, end = vertex.size(); i < end; i += 3) {
    vertex[i] += value_;
  }
}

// TranslateZCommand
TranslateZCommand::TranslateZCommand(double v) : value_(v) {}
TranslateZCommand::~TranslateZCommand(){};
void TranslateZCommand::execute(Data &data) {
  if (!value_) return;
  std::vector<double> &vertex = data.vertex;
  for (size_t i = 2, end = vertex.size(); i < end; i += 3) {
    vertex[i] += value_;
  }
}
// for pattern Memento
SaveCommand::SaveCommand(Memento **m) : m_(m){};
SaveCommand::~SaveCommand(){};
void SaveCommand::execute(Data &data) {
  if (data.vertex.empty()) return;
  try {
    if (!*m_) {
      *m_ = new Memento(data);
    } else {
      (*m_)->SetState(data);
    }
  } catch (...) {
    delete (*m_);
    *m_ = nullptr;
  }
}
LoadCommand::LoadCommand(Memento **m) : m_(m){};
LoadCommand::~LoadCommand(){};
void LoadCommand::execute(Data &data) {
  try {
    if (*m_) data = (*m_)->GetState();
  } catch (...) {
  }
}
}  // end namespace s21
