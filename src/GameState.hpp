#include <string>

enum StateName {
  menu,
  playing,
  paused
};

class GameState {
  public:
    explicit GameState(const StateName& init_state) : state(init_state) {};
    const StateName& get_state() { return state; };
    std::string get_state_string() const {
      switch(state) {
        case StateName(menu):
          return "Menu";
        case StateName(playing):
          return "Playing";
        case StateName(paused):
          return "Paused";
        default:
          return "Unknown";
      }
    }
  private:
    StateName state;
};
