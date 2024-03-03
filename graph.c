#include "includes/common.h"

// Function to create a new node
graph_t       *graph_create_node(SDL_Rect rect, coord_t center, int is_corridor, int id) {
  graph_t     *new_node = (graph_t *)malloc(sizeof(graph_t));
  if (new_node == NULL) {
    printf("Memory allocation failed\n");
    exit(EXIT_FAILURE);
  }

  new_node->rect = rect;
  new_node->center = center;
  new_node->is_corridor = is_corridor;
  new_node->id = id;
  new_node->parent = NULL;
  new_node->num_children = 0;
  new_node->children = NULL;

  return new_node;
}

// Function to add a child to a parent node
void          graph_add_child(graph_t *parent, graph_t *child) {
  parent->num_children++;

  parent->children = (graph_t **)realloc(parent->children, parent->num_children * sizeof(graph_t *));
  if (parent->children == NULL) {
    printf("Memory allocation failed\n");
    exit(EXIT_FAILURE);
  }
  parent->children[parent->num_children - 1] = child;
  child->parent = parent;
}

graph_t       *graph_create_node_from_room(state_t *state, con_t *connection, room_t *room) {
  graph_t     *room_node;
  con_t       *current_connection;
  room_t      *connected_room;
  graph_t     *connected_room_node;

  room_node = graph_create_node(room->room, room->center, 0, room->id);
  connected_room = NULL;
  connected_room_node = NULL;
  current_connection = connection;

  while (current_connection) {
    if (room->id == current_connection->src) {
      connected_room = find_room_by_id(state, current_connection->dst);
    } else if (room->id == current_connection->dst) {
      connected_room = find_room_by_id(state, current_connection->src);
    }
    if (connected_room != NULL) {
      connected_room_node = graph_create_node_from_room(state, current_connection->next, connected_room);
      graph_add_child(room_node, connected_room_node);
      connected_room = NULL;
      connected_room_node = NULL;
    }
    current_connection = current_connection->next;
  }

  return room_node;
}

void          graph_create(state_t *state) {
  graph_t     *current_node;
  room_t      *current_room;
  free_graph(state);
  state->graph = NULL;
  current_node = NULL;
  current_room = state->rooms;
  
  // while (current_room) {
    current_node = graph_create_node_from_room(state, state->connections, state->rooms);

    // if (state->graph == NULL) {
    state->graph = current_node;
  //   } else {
  //     graph_add_child(state->graph, current_node);
  //   }

  //   current_room = current_room->next;
  // }
  displayGraph(state->graph, 0);
  // free_rooms(state);
}

void          free_graph(state_t *state) {
  return;
}

void displayGraph(graph_t *node, int depth) {
    if (node == NULL) {
        return;
    }

    // Print the current node with indentation based on depth
    for (int i = 0; i < depth; i++) {
        printf("  ");
    }
    printf("Node: %d\n", node->id);

    // Print parent if exists
    if (node->parent != NULL) {
        for (int i = 0; i < depth + 1; i++) {
            printf("  ");
        }
        printf("Parent: %d\n", node->parent->id);
    }

    // Print children
    for (int i = 0; i < node->num_children; i++) {
        displayGraph(node->children[i], depth + 1); // Recursive call with increased depth
    }
}
// // Function to display the graph
// void displayGraph(graph_t *node) {
//   if (node == NULL) {
//     return;
//   }

//   printf("Node: %d\n", node->id);

//   if (node->parent != NULL) {
//     printf("  Parent: %d\n", node->parent->id);
//   }

//   for (int i = 0; i < node->num_children; i++) {
//     printf("  Child: %d\n", node->children[i]->id);
//   }

//   for (int i = 0; i < node->num_children; i++) {
//     displayGraph(node->children[i]);
//   }
// }

