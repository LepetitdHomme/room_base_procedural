#include "includes/common.h"

int find_subset(subset_t subsets[], int i) {
  if (subsets[i].parent != i)
    subsets[i].parent = find_subset(subsets, subsets[i].parent);
  return subsets[i].parent;
}

void union_subsets(subset_t subsets[], int x, int y) {
    int xroot = find_subset(subsets, x);
    int yroot = find_subset(subsets, y);

    if (subsets[xroot].rank < subsets[yroot].rank)
        subsets[xroot].parent = yroot;
    else if (subsets[xroot].rank > subsets[yroot].rank)
        subsets[yroot].parent = xroot;
    else {
        subsets[yroot].parent = xroot;
        subsets[xroot].rank++;
    }
}
// Comparison function for sorting edges by weight
int compare_edges(const void* a, const void* b) {
  edge_t* edge1 = (edge_t*)a;
  edge_t* edge2 = (edge_t*)b;
  if (edge1->weight < edge2->weight)
    return -1;
  else if (edge1->weight > edge2->weight)
    return 1;
  else
    return 0;
}

void link_rooms(state_t *state, int src, int dst) {
  room_t *tmp = state->rooms;
  room_t *found_src = NULL;
  room_t *found_dst = NULL;

  while (tmp) { // do this in doors_append ?
    if (tmp->id == src) {
      found_src = tmp;
    } else if (tmp->id == dst) {
      found_dst = tmp;
    }

    tmp = tmp->next;
  }
  doors_append(state, found_src, found_dst);
}

void printf_subsets(subset_t subsets[], int num_rooms) {
  printf("Subset information:\n");
  for (int i = 0; i < num_rooms; i++) {
    printf("Subset %d: parent = %d, rank = %d\n", i, subsets[i].parent, subsets[i].rank);
  }
}

void print_edges(edge_t edges[], int edge_count) {
  printf("Edges information:\n");
  for (int i = 0; i < edge_count; i++) {
    printf("Edge %d: src = %d, dst = %d, weight = %lf\n", i, edges[i].src, edges[i].dst, edges[i].weight);
  }
}

void display_added_array(bool *added, int size) {
  printf("Added array:\n");
  for (int i = 0; i < size; i++) {
      printf("Index %d: %s\n", i, added[i] ? "true" : "false");
  }
}

// Function to apply Kruskal's algorithm
void apply_kruskal(state_t *state) {
  room_t *current_room = state->rooms;
  room_t *other_room = NULL;
  int num_rooms = state->num_rooms;

  edge_t *edges = (edge_t*)malloc(num_rooms * (num_rooms - 1) / 2 * sizeof(edge_t));
  int edge_count = 0;

  /** construct and sort edges of graph **/
  while (current_room != NULL) {
    other_room = current_room->next;
    while (other_room != NULL) {
      edges[edge_count].src = current_room->id;
      edges[edge_count].dst = other_room->id;
      edges[edge_count].weight = distance_between_coords(current_room->center, other_room->center);
      edge_count++;
      other_room = other_room->next;
    }
    current_room = current_room->next;
  }
  qsort(edges, edge_count, sizeof(edge_t), compare_edges);

  // print_edges(edges, edge_count);

  /******** prepare Kruskal ***********/
  subset_t* subsets = (subset_t*)malloc(num_rooms * sizeof(subset_t));
  for (int v = 0; v < num_rooms; ++v) {
    subsets[v].parent = v;
    subsets[v].rank = 0;
  }
  int added_edges = 0;
  int i = 0;
  bool* added = (bool*)calloc(num_rooms, sizeof(bool));


  /** Apply Kruskal's algorithm until all rooms are connected **/
  while (added_edges < num_rooms - 1 && i < edge_count) {
    edge_t next_edge = edges[i++];

    int x = find_subset(subsets, next_edge.src);
    int y = find_subset(subsets, next_edge.dst);

    if (x != y) {
      added_edges++;
      added[next_edge.src] = true;
      added[next_edge.dst] = true;
      union_subsets(subsets, x, y);
      link_rooms(state, next_edge.src, next_edge.dst);
      connections_append(state, next_edge.src, next_edge.dst);
      // printf_subsets(subsets, num_rooms);
    }
  }
  // display_added_array(added, num_rooms);
  free(edges);
  free(subsets);
  free(added);
}
