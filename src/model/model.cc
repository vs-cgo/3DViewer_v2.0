#include "model.h"

namespace s21 {
// Ctor Model
Model::Model(){};
// Dtor Model
Model::~Model(){};

// Parsing
void Model::Parsing(const std::string &input) { pars_.Parsing(input, data_); };

// Run
void Model::Run(Command *c) {
  c->execute(data_);
  delete c;
}

// Clean
/*
void Model::Clean() {
  data_.vertex.clear();
  data_.polygon.clear();
  data_.minmax.clear();
  data_.edges = 0;
}
*/
// GetData
Data &Model::GetData() { return data_; }

}  // end namespace s21
