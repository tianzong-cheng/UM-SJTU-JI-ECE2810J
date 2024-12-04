#include <cstdint>
#include <algorithm>
#include <functional>
#include <iostream>
#include <queue>
#include <string>
#include <unordered_set>
#include <thread>
#include <vector>

typedef uint16_t Coord;
typedef std::pair<Coord, Coord> Point;
typedef std::pair<uint8_t, std::vector<uint8_t>> Route;

struct State {
    Point player;
    std::vector<Point> boxes;
    int cost;

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

    bool operator<(const State &other) const {
      return cost > other.cost;  // Min heap
    }

    struct Hash {
        size_t operator()(const State &state) const {
          std::hash<Coord> hasher;
          size_t h = 0;
          const size_t kGoldenRatio = 0x9e3779b9;

          h ^= hasher(state.player.first) + kGoldenRatio + (h << 6) + (h >> 2);
          h ^= hasher(state.player.second) + kGoldenRatio + (h << 6) + (h >> 2);

          for (const auto &box: state.boxes) {
            h ^= hasher(box.first) + kGoldenRatio + (h << 6) + (h >> 2);
            h ^= hasher(box.second) + kGoldenRatio + (h << 6) + (h >> 2);
          }

          return h;
        }
    };
};

const std::vector<Point> kDelta = {{-1, 0},
                                   {0,  1},
                                   {1,  0},
                                   {0,  -1}};

bool PointCompare(const Point &a, const Point &b) {
  return a.first > b.first || (a.first == b.first && a.second > b.second);
}

// Sort box vector using insertion sort
void SortBoxes(std::vector<Point> &boxes) {
  for (size_t i = 1; i < boxes.size(); i++) {
    size_t j = i;
    while (j > 0 && PointCompare(boxes[j - 1], boxes[j])) {
      std::swap(boxes[j], boxes[j - 1]);
      j--;
    }
  }
}

bool CheckTarget(const std::vector<std::string> &grid, const State &state) {
  for (const auto &box: state.boxes) {
    if (grid[box.first][box.second] != 'T' &&
        grid[box.first][box.second] != 'R') {
      return false;
    }
  }
  return true;
}

bool CheckEmpty(const std::vector<std::string> &grid, const State &state, const Point &point) {
  if (grid[point.first][point.second] == '#') {
    return false;
  }
  for (const auto &box: state.boxes) {
    if (box == point) {
      return false;
    }
    if (PointCompare(box, point)) {
      return true;
    }
  }
  return true;
}

bool CheckBoxCorner(const std::vector<std::string> &grid, const Point &box) {
  if (grid[box.first][box.second] == 'T' ||
      grid[box.first][box.second] == 'R') {
    return false;
  }

  if (grid[box.first - 1][box.second] == '#' &&
      grid[box.first][box.second - 1] == '#') {
    return true;
  }
  if (grid[box.first - 1][box.second] == '#' &&
      grid[box.first][box.second + 1] == '#') {
    return true;
  }
  if (grid[box.first + 1][box.second] == '#' &&
      grid[box.first][box.second - 1] == '#') {
    return true;
  }
  if (grid[box.first + 1][box.second] == '#' &&
      grid[box.first][box.second + 1] == '#') {
    return true;
  }

  return false;
}

bool CheckBoxWall(const Point &box, const std::vector<std::string> &grid) {
  if (grid[box.first][box.second] == 'T' ||
      grid[box.first][box.second] == 'R') {
    return false;
  }

  if (grid[box.first - 1][box.second] == '#') {
    bool possible = false;
    Coord i = box.second;
    while (i > 1 && grid[box.first][i] != '#' && !possible) {
      i--;
      if (grid[box.first][i] == 'T' || grid[box.first][i] == 'R' || grid[box.first - 1][i] != '#') {
        possible = true;
      }
    }
    i = box.second;
    while (i < grid[0].size() - 2 && grid[box.first][i] != '#' && !possible) {
      i++;
      if (grid[box.first][i] == 'T' || grid[box.first][i] == 'R' || grid[box.first - 1][i] != '#') {
        possible = true;
      }
    }
    if (!possible) {
      return true;
    }
  }
  if (grid[box.first + 1][box.second] == '#') {
    bool possible = false;
    Coord i = box.second;
    while (i > 1 && grid[box.first][i] != '#' && !possible) {
      i--;
      if (grid[box.first][i] == 'T' || grid[box.first][i] == 'R' || grid[box.first + 1][i] != '#') {
        possible = true;
      }
    }
    i = box.second;
    while (i < grid[0].size() - 2 && grid[box.first][i] != '#' && !possible) {
      i++;
      if (grid[box.first][i] == 'T' || grid[box.first][i] == 'R' || grid[box.first + 1][i] != '#') {
        possible = true;
      }
    }
    if (!possible) {
      return true;
    }
  }
  if (grid[box.first][box.second - 1] == '#') {
    bool possible = false;
    Coord i = box.first;
    while (i > 1 && grid[i][box.second] != '#' && !possible) {
      i--;
      if (grid[i][box.second] == 'T' || grid[i][box.second] == 'R' || grid[i][box.second - 1] != '#') {
        possible = true;
      }
    }
    i = box.first;
    while (i < grid.size() - 2 && grid[i][box.second] != '#' && !possible) {
      i++;
      if (grid[i][box.second] == 'T' || grid[i][box.second] == 'R' || grid[i][box.second - 1] != '#') {
        possible = true;
      }
    }
    if (!possible) {
      return true;
    }
  }
  if (grid[box.first][box.second + 1] == '#') {
    bool possible = false;
    Coord i = box.first;
    while (i > 1 && grid[i][box.second] != '#' && !possible) {
      i--;
      if (grid[i][box.second] == 'T' || grid[i][box.second] == 'R' || grid[i][box.second + 1] != '#') {
        possible = true;
      }
    }
    i = box.first;
    while (i < grid.size() - 2 && grid[i][box.second] != '#' && !possible) {
      i++;
      if (grid[i][box.second] == 'T' || grid[i][box.second] == 'R' || grid[i][box.second + 1] != '#') {
        possible = true;
      }
    }
    if (!possible) {
      return true;
    }
  }
  return false;
}

void PrintState(const std::vector<std::string> &terrain, const State &state) {
  auto terrain_copy = terrain;
  terrain_copy[state.player.first][state.player.second] = 'P';
  for (const auto &box: state.boxes) {
    if (terrain_copy[box.first][box.second] == 'T') {
      terrain_copy[box.first][box.second] = 'R';
    } else {
      terrain_copy[box.first][box.second] = 'B';
    }
  }
  for (const auto &row: terrain_copy) {
    std::cout << row << std::endl;
  }
  std::cout << std::endl;
}

std::string GetRouteString(const Route &route) {
  std::string route_string;
  for (auto it = route.second.begin(); it != route.second.end(); it++) {
    for (int i = 0; i < (it + 1 == route.second.end() ? route.first : 4); i++) {
      int move = (*it & (3 << (i * 2))) >> (i * 2);
      route_string.push_back("URDL"[move]);
    }
  }
  return route_string;
}

Route AppendRoute(const Route &route, char move_char) {
  uint8_t move;
  switch (move_char) {
    case 'U':
      move = 0;
      break;
    case 'R':
      move = 1;
      break;
    case 'D':
      move = 2;
      break;
    case 'L':
      move = 3;
      break;
    default:
      move = 0;
      break;
  }
  Route new_route = route;
  if (route.first == 4) {
    new_route.first = 1;
    new_route.second.push_back(move);
  } else {
    new_route.second.back() |= move << ((new_route.first) * 2);
    new_route.first++;
  }
  return new_route;
}

std::vector<Point> GetTargets(const std::vector<std::string> &grid) {
  std::vector<Point> targets;
  for (size_t i = 0; i < grid.size(); i++) {
    for (size_t j = 0; j < grid[i].size(); j++) {
      if (grid[i][j] == 'T' || grid[i][j] == 'R') {
        targets.emplace_back(i, j);
      }
    }
  }
  return targets;
}

int GetCost(const std::vector<Point> &targets, const State &state) {
  int cost = 0;
  for (const auto &box: state.boxes) {
    int min_cost = INT32_MAX;
    for (const auto &target: targets) {
      int dist = abs(box.first - target.first) + abs(box.second - target.second);
      if (dist < min_cost) {
        min_cost = dist;
      }
    }
    cost += min_cost;
  }
  return cost;
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
  std::cin >> m >> n;
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
  auto targets = GetTargets(grid);
  std::unordered_map<State, Route, State::Hash> visited;
  State start;
  int target_count = 0;
  int box_count = 0;
  bool start_found = false;
  Coord target_x_min, target_x_max, target_y_min, target_y_max;
  for (size_t i = 0; i < grid.size(); i++) {
    for (size_t j = 0; j < grid[i].size(); j++) {
      if (grid[i][j] == 'S') {
        if (start_found) {
          return "No solution!";
        }
        start.player = {i, j};
        start_found = true;
      } else if (grid[i][j] == 'B' || grid[i][j] == 'R') {
        start.boxes.emplace_back(i, j);
        if (CheckBoxCorner(grid, {i, j})) {
          return "No solution!";
        }
        box_count++;
      }

      if (grid[i][j] == 'T' || grid[i][j] == 'R') {
        target_count++;
        if (target_count == 1) {
          target_x_min = target_x_max = static_cast<Coord>(i);
          target_y_min = target_y_max = static_cast<Coord>(j);
        } else {
          target_x_min = std::min(target_x_min, static_cast<Coord>(i));
          target_x_max = std::max(target_x_max, static_cast<Coord>(i));
          target_y_min = std::min(target_y_min, static_cast<Coord>(j));
          target_y_max = std::max(target_y_max, static_cast<Coord>(j));
        }
      }
    }
  }
  if (target_count < box_count) {
    return "No solution!";
  }
  SortBoxes(start.boxes);
  start.cost = GetCost(targets, start);
  std::priority_queue<State> q;
  q.push(start);
  Route route = {4, {}};
  visited.insert({start, route});

  while (!q.empty()) {
    auto head = q.top();
    q.pop();
    auto player = head.player;
    for (size_t i = 0; i < kDelta.size(); i++) {
      auto next_player = player;
      next_player.first += kDelta[i].first;
      next_player.second += kDelta[i].second;

      if (grid[next_player.first][next_player.second] == '#') {
        continue;
      }

      State next_state = head;
      next_state.player = next_player;

      // Check if the box can be pushed
      bool failure = false;
      bool moved = false;
      Point moved_box;
      for (auto &box: next_state.boxes) {
        if (PointCompare(box, next_player)) {
          continue;
        }
        if (box == next_player) {
          box.first += kDelta[i].first;
          box.second += kDelta[i].second;
          moved = true;
          moved_box = box;
          if (!CheckEmpty(grid, head, box)) {
            failure = true;
            break;
          }
        }
      }
      if (failure || (moved && (CheckBoxCorner(grid, moved_box) ||
                                CheckBoxWall(moved_box, grid)))) {
        continue;
      }

      if (moved) {
        SortBoxes(next_state.boxes);
      }
      if (CheckTarget(grid, next_state)) {
        return GetRouteString(visited[head]) + "URDL"[i];
      }
      if (visited.find(next_state) == visited.end()) {
        visited.insert({next_state, AppendRoute(visited[head], "URDL"[i])});
        next_state.cost = GetCost(targets, next_state) +
                          (static_cast<int>(visited[next_state].second.size()) - 1) * 4 + visited[next_state].first;
        q.push(next_state);
      }
    }
  }

  return "No solution!";
}

// For big cases, paste your answers here (i.e. replace "ans for big 1" with
// your answer) Do not remove the first element, it should be left blank. NOTE:
// Ensure the order is correct! Your answer should look like "UUDDLLRR..."
const std::vector<std::string> answers = {
        "__leave_this_blank__",
        "UUUUULLLLULDRDLLLLLLLLRRUULLLLRRDDDLUDLLULDDDDLDRRRRRLDDDRRRDDDDRRRRDRRULLLRDDDLLUUURULLDLUUULURRR",
        "RUULDDLDDLLUULUUURRDLDDRRDDRRUULUULLULLDDRURRRULRDDDRDLDLLLUURRDRUUDDDLL",
        "DRURRLLUUULUURDRRRDDRDRDDLLUURRUURRUULLDDDDLLDDRRURULDDLLULLLUUULUURDRRRRLDDRRDDDLLULLLUUULURRRRDDRRDDDLLDLURRRUUULLDDUUUULLLDDDDRRDRUUURRDDDLRUUUURRUULLDLLLLRRRRDDLLUDDDLDDRUUUURRDLULDDLDDRURRURULULLDDLDRUUURRUULLDDDDLLLUUULUURDRRURDDDDULDDLLUUULURRRURDRRDDDLLURRUULLDDLDURRDL",
        "ans for big 4",
        "RRUUUULURRRRRRRRRURDDDDRDLDLLURDRUUUURULLLDLUUULUURRDLLDDDLLLLLDDDRDRRDRRULLLLDLUUUULURRRRRRRRRURDDDDRDLDLLURDRUUUURULLLDLUUULULLULDRRRURRDLLDDDLLLLLDDDRDRRDLLRRDDLLLUUUUUULURRRRRRRRRURDDDDRDLDLLURDRUUUURULLLDLUUULULLDLURRRURRDLLDDDLLLLLDDDDDLLLUURRDRUUULURRRRRRRRRURDDDDRDLDLLURDRUUUURULLLDLUUULULLLLRRRRURRDLLLLLRUL",
        "URRUULULLRRDRDDLUUDDLDLLURUURUURRDLDDRDDLLRURUULUURDDLLLDDRDRRULUURDULLLDDRDRUURULDDDRULLLLDRRURUURUULLLDDDUUURRRDDLUDDDLDLLURUURURRDLDDRDLLLURDRUUURULDDDRUU",
        "ans for big 7",
        "ULDDDRDLLLDLLURRRLLUURDUURURRDLDDLLUURURDLLDDRDLDLLULUURRLURRDLLLDDRDRRULLRRRURRDLLLDLLURURRDRRULDLLDLURUUURURRDLDDRDLLLUUURURDDDUULLDDDRRLLDLLURRUUULLDRDUURRRDDLLDDLLURURRRUULLLLDLDLDRUURURRDLDRLDDRUULUURRRDDLRDL",
        "RDDLLLDDLLURDRRRDRUUULLLDLDRRLUURRRDDDLULLULLUUUUURRDDDDDLDRRRURDUUUULUURDDDDLDDLLULLUUUURURDDDDDLDRRRLLULLUURLUURRDDDDLDRRLUURRDD",
        "DDRUUUULDDDRDLLLRURULLLULUURDDLDDRUUURR"};

// Don't modify this.
std::string print_answer(int index) {
  if (index < 1 || index >= (int) answers.size()) {
    return "invalid";
  }
  return answers[(size_t) index];
}

std::vector<std::string> GetTerrain(std::vector<std::string> &grid) {
  std::vector<std::string> terrain;
  for (size_t i = 0; i < grid.size(); i++) {
    std::string row;
    for (size_t j = 0; j < grid[i].size(); j++) {
      if (grid[i][j] == '#') {
        row.push_back('#');
      } else if (grid[i][j] == 'T' || grid[i][j] == 'R') {
        row.push_back('T');
      } else {
        row.push_back(' ');
      }
    }
    terrain.push_back(row);
  }
  return terrain;
}

void Play(std::vector<std::string> &grid, std::string &answer) {
  auto terrain = GetTerrain(grid);
  State state;
  for (size_t i = 0; i < grid.size(); i++) {
    for (size_t j = 0; j < grid[i].size(); j++) {
      if (grid[i][j] == 'S') {
        state.player = {i, j};
      } else if (grid[i][j] == 'B' || grid[i][j] == 'R') {
        state.boxes.emplace_back(i, j);
      }
    }
  }
  SortBoxes(state.boxes);

  std::cout << "\033[H\033[J";
  std::cout << answer << std::endl;
  std::cout << "Move: " << -1 << std::endl;
  PrintState(terrain, state);
  std::this_thread::sleep_for(std::chrono::seconds(1));

  for (size_t i = 0; i < answer.size(); i++) {
    auto player = state.player;
    auto next_player = player;
    switch (answer[i]) {
      case 'U':
        next_player.first--;
        break;
      case 'D':
        next_player.first++;
        break;
      case 'L':
        next_player.second--;
        break;
      case 'R':
        next_player.second++;
        break;
      default:
        break;
    }
    for (auto &box: state.boxes) {
      if (PointCompare(box, next_player)) {
        break;
      }
      if (box == next_player) {
        box.first += next_player.first - player.first;
        box.second += next_player.second - player.second;
        break;
      }
    }
    SortBoxes(state.boxes);
    state.player = next_player;

    std::cout << "\033[H\033[J";
    std::cout << answer << std::endl;
    std::cout << "Move: " << i << std::endl;
    PrintState(terrain, state);
    std::this_thread::sleep_for(std::chrono::seconds(1));
  }
}
