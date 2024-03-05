#include "includes/common.h"

// Function to create a new node
graph_t       *graph_create_node(SDL_Rect rect, coord_t center, int is_corridor, int elevation, int id) {
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
  new_node->elevation = elevation;
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
  door_t      door_node_from_parent;
  door_t      door_node_from_child;
  door_t      door_node_child;
  SDL_Rect    corridor_rect;
  int         room_validation;
  int         elevation;

  // create door on parent
  door_node_parent = door_coordinates(parent_node, child_node);
  // create door on child
  door_node_child = door_coordinates(child_node, parent_node);

  // create corridor rect
  elevation = parent_node->elevation;
  corridor_rect = rect_from_doors(door_node_parent, door_node_child);
  if ((room_validation = room_is_valid(state, corridor_rect, 0, MIN_CORRIDOR_SIZE)) != 0) {
    // printf("corridor invalid: %d\n", room_validation);
    if (room_validation == 2) { // no place to walk
      if (corridor_rect.w >= corridor_rect.h) {
        corridor_rect.h += 1;
        door_node_child.coord.y += 1;
      } else {
        corridor_rect.w += 1;
        door_node_child.coord.x += 1;
      }
    } else if (room_validation == 1) { // corridor on another room
      elevation = -1;
    }
  }

  // persist doors on parent and child
  doors_append(parent_node, door_node_parent);
  doors_append(child_node, door_node_child);

  // creat corridor node
  corridor_node = graph_create_node(corridor_rect, room_center(corridor_rect), 1, elevation, -1);
  // get doors for middle room = corridor
  door_node_from_parent.coord = next_coord_with_step(door_node_parent.coord, door_node_parent.dir);
  door_node_from_parent.dir = invert_dir(door_node_parent.dir);
  door_node_from_child.coord = next_coord_with_step(door_node_child.coord, door_node_child.dir);
  door_node_from_child.dir = invert_dir(door_node_child.dir);
  doors_append(corridor_node, door_node_from_parent);
  doors_append(corridor_node, door_node_from_child);

  return corridor_node;
}

/* creates a node for the current room
*  go through connections, and recursively calls this function for connected rooms
*  to avoid recreating nodes, we memcpy the connections array without the parent index
*/
graph_t       *graph_create_node_from_room(state_t *state, con_t *connections, int num_connections, room_t *room) {
  graph_t     *room_node;
  con_t       *new_connections;
  room_t      *connected_room;
  graph_t     *connected_room_node;
  graph_t     *corridor_node;

  new_connections = NULL;
  room_node = graph_create_node(room->room, room->center, 0, 0, room->id);

   for (int i = 0; i < num_connections; i++)  {
    connected_room = NULL;
    connected_room_node = NULL;
    
    if (room->id == connections[i].src) {
      connected_room = find_room_by_id(state, connections[i].dst);

    } else if (room->id == connections[i].dst) {
      connected_room = find_room_by_id(state, connections[i].src);
    }
    if (connected_room != NULL) {
      /* We create a copy of connections struct array without current index */
      new_connections = (con_t *)malloc((num_connections - 1) * sizeof(con_t));
      if (new_connections == NULL) {
        printf("Memory allocation failed\n");
        exit(EXIT_FAILURE);
      }
      int k = 0;
      for (int j = 0; j < num_connections; j++) {
        if (i != j) { // Excludes the current connection !
          new_connections[k] = connections[j];
          k++;
        }
      }
      /********************************************************************/

      connected_room_node = graph_create_node_from_room(state, new_connections, k, connected_room);
      corridor_node = graph_create_node_from_connection(state, room_node, connected_room_node);
      free(new_connections);
      new_connections = NULL;
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
  // graph_print(state->graph, 0);
  free_rooms(state);
}

void          free_node(graph_t *node) {
  if (node == NULL) {
    return;
  }

  for (int i = 0 ; i < node->num_children ; i++) {
    free_node(node->children[i]);
  }

  free(node->doors);
  node->doors = NULL;
  free(node);
}

void          free_graph(state_t *state) {
  free_node(state->graph);
  state->graph = NULL;
}

void          graph_print(graph_t *node, int depth) {
  if (node == NULL) {
    return;
  }

  // Print the current node with indentation based on depth
  for (int i = 0; i < depth; i++) {
    printf("  ");
  }
  if (node->is_corridor == 1) {
    printf("Corridor: id:%d, elevation: %d, num_doors: %d\n", node->id, node->elevation, node->num_doors);
  } else {
    printf("Node: id:%d, elevation: %d, num_doors: %d\n", node->id, node->elevation, node->num_doors);
  }

  // Print parent if exists
  // if (node->parent != NULL) {
  //   for (int i = 0; i < depth + 1; i++) {
  //     printf("  ");
  //   }
  //   printf("Parent: %d\n", node->parent->id);
  // }

  // Print children
  for (int i = 0; i < node->num_children; i++) {
    graph_print(node->children[i], depth + 1); // Recursive call with increased depth
  }
}


