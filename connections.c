#include "includes/common.h"

void                    connections_append(state_t *state, int src, int dst) {
  // replacing head instead of fetching tail
  con_t *new_node = (con_t *)malloc(sizeof(con_t));

  if (new_node == NULL) {
    DEBUG_MSG("Error during connection_append#malloc");
    exit(EXIT_FAILURE);
  }

  new_node->src = src;
  new_node->dst = dst;
  new_node->next = state->connections;
  state->connections = new_node;
}

void                    connections_print(state_t *state) {
  con_t *tmp = state->connections;

  while (tmp) {
    printf("connection: %d-%d\n", tmp->src, tmp->dst);
    tmp = tmp->next;
  }
}

void                    free_connections(state_t *state) {
  con_t *current = state->connections;
  con_t *next = NULL;

  while (current) {
    next = current->next;
    free(current);
    current = next;
  }

  state->connections = NULL;
}