#include "./memento.h"

namespace s21 {
Memento::Memento(Data &data) : state_(data){};
void Memento::SetState(Data const &data) { state_ = data; }
Data Memento::GetState() { return state_; };

}  // end namespace s21
