/*
 * Copyright (C) 2013  Mathieu Desnoyers <mathieu.desnoyers@efficios.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <stdexcept>
#include <thread>

#include <urcu/compiler.h> /* For CAA_ARRAY_SIZE */
#include <urcu/rculist.h>  /* List example */
#include <urcu/urcu-bp.h>  /* Bulletproof RCU flavor */

/*
 * Example showing how to use the Bulletproof Userspace RCU flavor.
 *
 * This is a mock-up example where updates and RCU traversals are
 * performed by the same thread to keep things simple on purpose.
 */

static CDS_LIST_HEAD(mylist);

struct mynode {
  uint64_t value;
  cds_list_head node;       /* linked-list chaining */
  struct rcu_head rcu_head; /* for call_rcu() */
};

static int add_node(uint64_t v) {
  mynode* node;

  node = reinterpret_cast<mynode*>(calloc(sizeof(*node), 1));
  if (!node) {
    return -1;
  }
  node->value = v;
  printf("Wrote value: %ld \n", node->value);
  cds_list_add_rcu(&node->node, &mylist);
  return 0;
}

mynode *node, *n;

void writer() {
  printf("Start writing!\n");
  uint64_t values[] = {
      42,
      36,
      24,
  };

  unsigned int i;
  int ret;

  /*
   * Notice that with the bulletproof flavor, there is no need to
   * register/unregister RCU reader threads.
   */

  /*
   * Adding nodes to the linked-list. Safe against concurrent
   * RCU traversals, require mutual exclusion with list updates.
   */
  for (i = 0; i < CAA_ARRAY_SIZE(values); i++) {
    ret = add_node(values[i]);
    if (ret == -1) {
      throw std::runtime_error("Failed to add node.");
    }
  }
  printf("Finish writing!\n");
}

void reader() {
  size_t counter = 0;
  for (;;) {
    void* p;
    rcu_dereference(p);

    /*
     * We need to explicitly mark RCU read-side critical sections
     * with rcu_read_lock() and rcu_read_unlock(). They can be
     * nested. Those are no-ops for the QSBR flavor.
     */
    urcu_bp_read_lock();

    /*
     * RCU traversal of the linked list.
     */
    cds_list_for_each_entry_rcu(node, &mylist, node) {
      printf("Read value: %ld \n", node->value);
      cds_list_del_rcu(&node->node);
      counter++;
    }
    urcu_bp_read_unlock();

    if (counter >= 3) {
      return;
    }
  }
}

int main(void) {
  std::thread reader_thread(&reader);
  std::thread writer_thread(&writer);

  reader_thread.join();
  writer_thread.join();

  /*
   * Removing nodes from linked list. Safe against concurrent RCU
   * traversals, require mutual exclusion with list updates.
   */
  cds_list_for_each_entry_safe(node, n, &mylist, node) {
    cds_list_del_rcu(&node->node);

    /*
     * Using synchronize_rcu() directly for synchronization
     * so we keep a minimal impact on the system by not
     * spawning any call_rcu() thread. It is slower though,
     * since there is no batching.
     */
    urcu_bp_synchronize_rcu();
    free(node);
  }
}
