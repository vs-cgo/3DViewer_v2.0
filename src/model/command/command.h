#ifndef _3DVIEWER_V2_0_MODEL_COMMAND_COMMAND_H_
#define _3DVIEWER_V2_0_MODEL_COMMAND_COMMAND_H_

#include <cmath>

#include "../../view/memento/memento.h"
#include "../data.h"

namespace s21 {
/**
 * @brief The Command class базовый класс от которого наследуются команды
 */
class Command {
 public:
  /**
   * @brief ~Command виртуальный деструктор для удаления производных классов по
   * указателю на базовый
   */
  virtual ~Command(){};
  /**
   * @brief execute чистая виртуальная функция, которую надо override в
   * производном классе
   * @param[in] d ссылка на структуру для выполнения преобразований над данными
   */
  virtual void execute(Data &d) = 0;
};

/**
 * @brief The ZoomCommand class команда для увеличения/уменьшения модели
 */
class ZoomCommand : public Command {
 public:
  /**
   * @brief ZoomCommand конструктор
   * @param[in] x значение на которое надо увеличить/уменьшить модель(больше 1
   * увеличивает, меньше 1 - уменьшает)
   */
  ZoomCommand(double x = 1);
  ~ZoomCommand();
  /**
   * @brief execute метод для выполнения команды
   * @param[in] d ссылка на структуру Data
   */
  void execute(Data &d) override;

 private:
  double value_;
};

/**
 * @brief The AroundXCommand class команда для поворота вокруг оси x
 */
class AroundXCommand : public Command {
 public:
  /**
   * @brief AroundXCommand конструктор
   * @param[in] a угол поворота в радианах(default = 0)
   */
  AroundXCommand(double a = 0);
  ~AroundXCommand();
  /**
   * @brief execute метод для выполнения команды
   * @param[in] d ссылка на структуру Data
   */
  void execute(Data &d) override;

 private:
  double angle_;
};

/**
 * @brief The AroundYCommand class для поворота вокруг оси y
 */
class AroundYCommand : public Command {
 public:
  /**
   * @brief AroundYCommand конструктор
   * @param[in] a угол поворота в радианах(default = 0)
   */
  AroundYCommand(double a = 1);
  ~AroundYCommand();
  /**
   * @brief execute метод для выполнения команды
   * @param[in] d ссылка на структуру Data
   */
  void execute(Data &d) override;

 private:
  double angle_;
};

/**
 * @brief The AroundZCommand class для поворота вокруг оси Z
 */
class AroundZCommand : public Command {
 public:
  /**
   * @brief AroundZCommand конструктор
   * @param[in] a угол поворота в радианах(default = 0)
   */
  AroundZCommand(double a = 1);
  ~AroundZCommand();
  /**
   * @brief execute метод для выполнения команды
   * @param[in] d ссылка на структуру Data
   */
  void execute(Data &d) override;

 private:
  double angle_;
};

/**
 * @brief The TranslateXCommand class для смещения вдоль оси x
 */
class TranslateXCommand : public Command {
 public:
  /**
   * @brief TranslateXCommand конструктор
   * @param[in] v значение смещения(default = 0)
   */
  TranslateXCommand(double v = 0);
  ~TranslateXCommand();
  /**
   * @brief execute метод для выполнения команды
   * @param[in] d ссылка на структуру Data
   */
  void execute(Data &d) override;

 private:
  double value_;
};

/**
 * @brief The TranslateYCommand class для смещения вдоль оси y
 */
class TranslateYCommand : public Command {
 public:
  /**
   * @brief TranslateYCommand конструктор
   * @param[in] v значение смещения(default = 0)
   */
  TranslateYCommand(double v = 0);
  ~TranslateYCommand();
  /**
   * @brief execute метод для выполнения команды
   * @param[in] d ссылка на структуру Data
   */
  void execute(Data &d) override;

 private:
  double value_;
};

/**
 * @brief The TranslateZCommand class для смещения вдоль оси z
 */
class TranslateZCommand : public Command {
 public:
  /**
   * @brief TranslateZCommand конструктор
   * @param[in] v значение смещения(default = 0)
   */
  TranslateZCommand(double v = 0);
  ~TranslateZCommand();
  /**
   * @brief execute метод для выполнения команды
   * @param[in] d ссылка на структуру Data
   */
  void execute(Data &d) override;

 private:
  double value_;
};

/**
 * @brief The SaveCommand class команда для сохранения текущих координат объекта
 */
class SaveCommand : public Command {
 public:
  /**
   * @brief SaveCommand конструктор
   * @param[in] m указатель на указатель на объект класса memento(default =
   * nullptr)
   */
  SaveCommand(Memento **m = nullptr);
  ~SaveCommand();
  /**
   * @brief execute метод для выполнения команды
   * @param[in] d ссылка на структуру Data
   */
  void execute(Data &d) override;

 private:
  Memento **m_;
};

/**
 * @brief The LoadCommand class команда для загрузки, если есть, данных в модель
 * из объекта memento
 */
class LoadCommand : public Command {
 public:
  /**
   * @brief LoadCommand конструктор
   * @param[in] m указатель на указатель на объект класса memento(default =
   * nullptr)
   */
  LoadCommand(Memento **m = nullptr);
  ~LoadCommand();
  /**
   * @brief execute метод для выполнения команды
   * @param[in] d ссылка на структуру Data
   */
  void execute(Data &d) override;

 private:
  Memento **m_;
};
};      // namespace s21
#endif  // _3DVIEWER_V2_0_MODEL_COMMAND_COMMAND_H_
