#include "sokoban.hpp"

int main() {
  std::vector<std::string> grid;
  read_map(grid);
  std::string ans = solve(grid);
  Play(grid, ans);
  return 0;
}
