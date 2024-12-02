#include <cstdint>
#include <functional>
#include <iostream>
#include <queue>
#include <string>
#include <unordered_set>
#include <vector>

typedef int16_t Coord;
typedef std::pair<Coord, Coord> Point;

struct State {
  Point player;
  std::vector<Point> boxes;

  bool operator==(const State &other) const {
    if (player.first != other.player.first ||
        player.second != other.player.second) {
      return false;
    }
    for (size_t i = 0; i < boxes.size(); i++) {
      if (boxes[i] != other.boxes[i]) {
        return false;
      }
    }
    return true;
  }

  struct Hash {
    size_t operator()(const State &state) const {
      std::hash<Coord> hasher;
      size_t h = 0;
      const size_t kGoldenRatio = 0x9e3779b9;

      h ^= hasher(state.player.first) + kGoldenRatio + (h << 6) + (h >> 2);
      h ^= hasher(state.player.second) + kGoldenRatio + (h << 6) + (h >> 2);

      for (const auto &box : state.boxes) {
        h ^= hasher(box.first) + kGoldenRatio + (h << 6) + (h >> 2);
        h ^= hasher(box.second) + kGoldenRatio + (h << 6) + (h >> 2);
      }

      return h;
    }
  };
};

bool PointCompare(const Point &a, const Point &b) {
  return a.first > b.first || (a.first == b.first && a.second > b.second);
}

// Sort box vector using insertion sort
void SortBoxes(std::vector<Point> &boxes) {
  for (size_t i = 1; i < boxes.size(); i++) {
    size_t j = i;
    while (j > 0 && PointCompare(boxes[j], boxes[j - 1])) {
      std::swap(boxes[j], boxes[j - 1]);
      j--;
    }
  }
}

/**
 * @brief  Read your map from stdin
 * @note   Input format: See project description
 * @param  &grid: You should first resize the grid! Modify it inplace.
 * @retval None
 */
void read_map(std::vector<std::string> &grid) {
  // Read the grid size
  size_t n, m;
  std::cin >> n >> m;
  grid.resize(n);
  for (size_t i = 0; i < n; i++) {
    std::cin >> grid[i];
  }
}

/**
 * @brief  Solve the sokoban described by the grid
 * @note   Output string format: See project description
 * @param  &grid: The grid you read by `read_map'
 * @retval
 */
std::string solve(std::vector<std::string> &grid) {
  std::unordered_set<State, State::Hash> visited;
  State start;
  for (size_t i = 0; i < grid.size(); i++) {
    for (size_t j = 0; j < grid[i].size(); j++) {
      if (grid[i][j] == 'S') {
        start.player = {i, j};
      } else if (grid[i][j] == 'B' || grid[i][j] == 'R') {
        start.boxes.push_back({i, j});
      }
    }
  }
  std::queue<State> q;
  q.push(start);
  visited.insert(start);
}

// For big cases, paste your answers here (i.e. replace "ans for big 1" with
// your answer) Do not remove the first element, it should be left blank. NOTE:
// Ensure the order is correct! Your answer should look like "UUDDLLRR..."
const std::vector<std::string> answers = {
    "__leave_this_blank__", "ans for big 1", "ans for big 2", "ans for big 3",
    "ans for big 4",        "ans for big 5", "ans for big 6", "ans for big 7",
    "ans for big 8",        "ans for big 9", "ans for big 10"};

// Don't modify this.
std::string print_answer(int index) {
  if (index < 1 || index >= (int)answers.size()) {
    return "invalid";
  }
  return answers[(size_t)index];
}