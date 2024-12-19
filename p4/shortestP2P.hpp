#include <climits>
#include <cstddef>
#include <iostream>
#include <list>
#include <vector>
// You are not allowed to include additional libraries

#define INF INT_MAX

using namespace std;

class ShortestP2P {
public:
  ShortestP2P() {}

  /* Read the graph from stdin
   * The input has the following format:
   *
   * Firstline: 1 unsigned int, the total number of verticies, X.
   * Secondline: 1 unsigned int, the total number of edges between vertices, Y.
   * A set of Y lines, each line contains tuple of 2 unsigned int and 1 int (not
   * unsigned int!), in the format of: A(point 1, unsigned) B(point 2, unsigned)
   * dist(distance, int)
   *
   * Example:
   * 4
   * 4
   * 0 1 5
   * 1 2 -1
   * 2 3 4
   * 3 1 6
   *
   *
   *
   *
   * Vertices that are not connected have an infinitly large distance. You may
   * use INF (previously defined at the top of this cope snippet) for infinitly
   * large distance.
   *
   * Special: when the graph is not proper, where there exist any pair of
   * vertices whose minimum distance does not exist, terminate immediately by
   * printing: cout << "Invalid graph. Exiting." << endl;
   *
   * Note: vertex pairs that are not connected, which have infinitely large
   * distances are not considered cases where "minimum distances do not exist".
   */
  void readGraph() {
    size_t v, e;
    cin >> v >> e;

    // Initialize edges to infinity and diagonal to 0
    dist = vector<vector<int>>(v, vector<int>(v, 0x7FFFFFFF));
    for (unsigned int i = 0; i < v; i++) {
      dist[i][i] = 0;
    }

    for (size_t i = 0; i < e; i++) {
      size_t start, end;
      int weight;
      cin >> start >> end >> weight;
      dist[start][end] = weight;
    }

    // Floyd
    for (size_t k = 0; k < v; k++) {
      for (size_t i = 0; i < v; i++) {
        for (size_t j = 0; j < v; j++) {
          if (dist[i][k] != INF && dist[k][j] != INF &&
              dist[i][j] > dist[i][k] + dist[k][j]) {
            dist[i][j] = dist[i][k] + dist[k][j];
          }
          if (i == j && dist[i][j] < 0) {
            cout << "Invalid graph. Exiting." << endl;
            exit(0);
          }
        }
      }
    }
  }

  /* Input: 2 vertices A and B
   * Output: distance between them.
   * cout << dist << endl;
   *
   * When the A and B are not connected print INF:
   * cout << "INF" << endl;
   */
  void distance(unsigned int A, unsigned int B) {
    if (dist[A][B] == 0x7FFFFFFF) {
      cout << "INF" << endl;
    } else {
      cout << dist[A][B] << endl;
    }
  }

private:
  // internal data and functions.
  vector<vector<int>> dist;
};
