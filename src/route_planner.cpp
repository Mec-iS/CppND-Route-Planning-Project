#include "route_planner.h"
#include <algorithm>
using std::sort;

RoutePlanner::RoutePlanner(RouteModel &model, float start_x, float start_y,
    float end_x, float end_y): m_Model(model) {
    // Convert inputs to percentage:
    start_x *= 0.01;
    start_y *= 0.01;
    end_x *= 0.01;
    end_y *= 0.01;

    // TODO 2: Use the m_Model.FindClosestNode method to find the closest nodes to the starting and ending coordinates.
    // Store the nodes you find in the RoutePlanner's start_node and end_node attributes.
    this->start_node = &m_Model.FindClosestNode(start_x, start_y);
    this->end_node = &m_Model.FindClosestNode(end_x, end_y); 
}


// TODO 3: Implement the CalculateHValue method.
// Tips:
// - You can use the distance to the end_node for the h value.
// - Node objects have a distance method to determine the distance to another node.

float RoutePlanner::CalculateHValue(RouteModel::Node const *node) {
    return node->distance(*this->end_node);
}


// TODO 4: Complete the AddNeighbors method to expand the current node by adding all unvisited neighbors to the open list.
// Tips:
// - Use the FindNeighbors() method of the current_node to populate current_node.neighbors vector with all the neighbors.
// - For each node in current_node.neighbors, set the parent, the h_value, the g_value. 
// - Use CalculateHValue below to implement the h-Value calculation.
// - For each node in current_node.neighbors, add the neighbor to open_list and set the node's visited attribute to true.

void RoutePlanner::AddNeighbors(RouteModel::Node *current_node) {
    current_node->FindNeighbors();

    for (auto n: current_node->neighbors) {
        n->parent = current_node;
        n->h_value = this->CalculateHValue(n);
        n->g_value = current_node->g_value + current_node->distance(*n);

        this->open_list.push_back(n);
        n->visited = true;
    }
    // current_node->FindNeighbors();
  
    //   for (RouteModel::Node *neighbor: current_node->neighbors) {
    //     neighbor->parent = current_node;
    //     neighbor->h_value = RoutePlanner::CalculateHValue(neighbor);
    //     neighbor->g_value = current_node->g_value + current_node->distance(*neighbor);
        
    //     RoutePlanner::open_list.emplace_back(neighbor);
        
    //     neighbor->visited = true;
    //   }

}


// TODO 5: Complete the NextNode method to sort the open list and return the next node.
// Tips:
// - Sort the open_list according to the sum of the h value and g value.
// - Create a pointer to the node in the list with the lowest sum.
// - Remove that node from the open_list.
// - Return the pointer.

bool Compare(const RouteModel::Node* a,
             const RouteModel::Node* b) {
  // Compare the F values of two cells.
  float f1 = a->h_value + a->g_value; // f1 = g1 + h1
  float f2 = b->h_value + b->g_value; // f2 = g2 + h2
  return f1 < f2; 
}

void NodeSort(std::vector<RouteModel::Node*> &v) {
  // Sort in descending order.
  sort(v.begin(), v.end(), Compare);
}

RouteModel::Node *RoutePlanner::NextNode() {
  NodeSort(this->open_list);
  // sort(RoutePlanner::open_list.begin(), RoutePlanner::open_list.end(), [](const auto &_1st, const auto &_2nd){
  //   return _1st->h_value + _1st->g_value < _2nd->h_value +_2nd->g_value;
  // });

    // clone last element and assign pointer
    RouteModel::Node *node_ptr = open_list.front();
    // clear the open list for next node's neighobors
    this->open_list.erase(this->open_list.begin());
    return node_ptr;
}

// RouteModel::Node RoutePlanner::NextNode() {
//     std::sort(open_list.begin(), open_list.end(), [](const auto &_1st, const auto &_2nd){
//     return _1st -> h_value + _1st -> g_value < _2nd -> h_value + _2nd -> g_value;
//     });
// }


// TODO 6: Complete the ConstructFinalPath method to return the final path found from your A* search.
// Tips:
// - This method should take the current (final) node as an argument and iteratively follow the 
//   chain of parents of nodes until the starting node is found.
// - For each node in the chain, add the distance from the node to its parent to the distance variable.
// - The returned vector should be in the correct order: the start node should be the first element
//   of the vector, the end node should be the last element.

std::vector<RouteModel::Node> RoutePlanner::ConstructFinalPath(RouteModel::Node *current_node) {
    // Create path_found vector
    distance = 0.0f;
    std::vector<RouteModel::Node> path_found;

    // TODO: Implement your solution here.
    RouteModel::Node current = *current_node;
    RouteModel::Node* parent = current.parent;
    while (true) {
        // store current node
        auto head = path_found.begin();
        path_found.insert(head, current);

        // look up its parent node
        if (!parent) { break; }

        distance += parent->distance(current);
        
        // go to next node
        current = *parent;
        parent = current.parent;
    }

    distance *= this->m_Model.MetricScale(); // Multiply the distance by the scale of the map to get meters.

    return path_found;
}


// TODO 7: Write the A* Search algorithm here.
// Tips:
// - Use the AddNeighbors method to add all of the neighbors of the current node to the open_list.
// - Use the NextNode() method to sort the open_list and return the next node.
// - When the search has reached the end_node, use the ConstructFinalPath method to return the final path that was found.
// - Store the final path in the m_Model.path attribute before the method exits. This path will then be displayed on the map tile.

void RoutePlanner::AStarSearch() {
    RouteModel::Node *current_node = nullptr;

    this->start_node->visited = true;
    this->open_list.push_back(RoutePlanner::start_node);
  
    while (!RoutePlanner::open_list.empty()) {
      current_node = this->NextNode();
      
      if (current_node->distance(*this->end_node) == 0.0) {
        this->m_Model.path = RoutePlanner::ConstructFinalPath(current_node);
        return;
      }
      
      this->AddNeighbors(current_node);
    }
}