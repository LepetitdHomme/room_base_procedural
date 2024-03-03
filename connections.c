#include "includes/common.h"

void                    connections_append(state_t *state, int src, int dst) {
  // Increase the size of the connections array
  state->connections = realloc(state->connections, (state->num_connections + 1) * sizeof(con_t));
  if (state->connections == NULL) {
      printf("Memory allocation failed!\n");
      exit(1);
  }

  // Add the new connection
  state->connections[state->num_connections].src = src;
  state->connections[state->num_connections].dst = dst;
  state->num_connections++;
}

void                    connections_print(state_t *state) {
  for (int i = 0 ; i < state->num_connections ; i++) {
    printf("connection %d - %d\n", state->connections[i].src, state->connections[i].dst);
  }
}

void                    free_connections(state_t *state) {
  free(state->connections);
  state->num_connections = 0;
  state->connections = NULL;
}