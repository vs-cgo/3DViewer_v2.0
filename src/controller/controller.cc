#include "controller.h"

namespace s21 {
// Ctor controlller
Controller::Controller() {}

// Dtor controller
Controller::~Controller() {}

// Parsing
void Controller::Parsing(const std::string &str) { model_.Parsing(str); }

// Run
void Controller::Run(Command *c) { model_.Run(c); }

// Get data for render
Data &Controller::GetData() { return model_.GetData(); };
}  // namespace s21
