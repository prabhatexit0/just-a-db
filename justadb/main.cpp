#include "storage/node.h"
#include "justadb/storage/linked_list.h"
#include "qp/qp.h"

int main() {
  using namespace Storage;

  JustADbQP::QP qp;
  qp.qp_hello_world();

  auto ll = JLinkedList<int>();
  for (int i = 0; i < 100; i++) {
    const auto node = std::make_shared<JNode<int>>(i);
    ll.add(node);
  }

  ll.print();
  std::cout << "\n -- \n -- \n";

  ll.remove([](const DataPtr<int>& a) -> bool {
    return *a == 1;
  });

  ll.print();
  std::cout << "\n -- \n -- \n";

  return 0;
}
