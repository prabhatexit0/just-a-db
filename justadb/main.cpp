#include "qp/qp.h"
#include "storage/storage.h"


int main() {
  JustADbStorage::Storage storage;
  JustADbQP::QP qp;

  storage.storage_hello_world();
  qp.qp_hello_world();

  return 0;
}