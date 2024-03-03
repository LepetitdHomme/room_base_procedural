#include "includes/common.h"

// Function to create a new node
graph_t       *graph_create_node(SDL_Rect rect, coord_t center, int is_corridor, int id) {
  graph_t     *new_node = (graph_t *)malloc(sizeof(graph_t));
  if (new_node == NULL) {
    printf("Memory allocation failed\n");
    exit(EXIT_FAILURE);
  }

  new_node->parent = NULL;
  new_node->children = NULL;
  new_node->num_children = 0;
  new_node->rect = rect;
  new_node->center = center;
  new_node->doors = NULL;
  new_node->num_doors = 0;
  new_node->is_corridor = is_corridor;
  new_node->id = id;

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

graph_t       *graph_create_node_from_connection(state_t *state, graph_t *parent_node, graph_t *child_node) {
  graph_t     *corridor_node;
  door_t      door_node_parent;
  door_t      door_node_child;
  SDL_Rect    corridor_rect;

  door_node_parent = door_coordinates(parent_node, child_node);
  doors_append(parent_node, door_node_parent);
  door_node_child = door_coordinates(child_node, parent_node);
  doors_append(child_node, door_node_child);

  corridor_rect = rect_from_doors(door_node_parent, door_node_child);
  corridor_node = graph_create_node(corridor_rect, room_center(corridor_rect), 1, state->num_rooms + 1);
  doors_append(corridor_node, door_node_parent);
  doors_append(corridor_node, door_node_child);

  return corridor_node;
}

/* creates a node for the current room
*  go through connections, and recursively calls this function for connected rooms
*  to avoid recreating nodes, we memcpy the connections array without the parent index
*/
graph_t       *graph_create_node_from_room(state_t *state, con_t *connections, int num_connections, room_t *room) {
  graph_t     *room_node;
  con_t       current_connection;
  room_t      *connected_room;
  graph_t     *connected_room_node;
  graph_t     *corridor_node;

  room_node = graph_create_node(room->room, room->center, 0, room->id);

   for (size_t i = 0; i < num_connections; i++)  {
    connected_room = NULL;
    connected_room_node = NULL;
    
    if (room->id == connections[i].src) {
      connected_room = find_room_by_id(state, connections[i].dst);

    } else if (room->id == connections[i].dst) {
      connected_room = find_room_by_id(state, connections[i].src);
    }
    if (connected_room != NULL) {
      /* We create a copy of connections struct array without current index */
      con_t *new_connections = (con_t *)malloc((num_connections - 1) * sizeof(con_t));
      if (new_connections == NULL) {
        printf("Memory allocation failed\n");
        exit(EXIT_FAILURE);
      }
      int k = 0;
      for (size_t j = 0; j < num_connections; j++) {
        if (i != j) { // Excludes the current connection !
          new_connections[k] = connections[j];
          k++;
        }
      }
      /********************************************************************/

      connected_room_node = graph_create_node_from_room(state, new_connections, k, connected_room);
      corridor_node = graph_create_node_from_connection(state, room_node, connected_room_node);
      free(new_connections);
      graph_add_child(room_node, corridor_node);
      graph_add_child(corridor_node, connected_room_node);
    }
  }

  return room_node;
}

void          graph_create(state_t *state) {
  free_graph(state);
  state->graph = NULL;
  
  /* we start with the first room, but we could start with a any random room post kruskal (state->connections) */
  state->graph = graph_create_node_from_room(state, state->connections, state->num_connections, state->rooms);
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


