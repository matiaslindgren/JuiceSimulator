#include <string>

enum StateName {
  menu,
  playing,
  paused
};

class GameState {
  public:
    explicit GameState(const StateName& init_state) : state_(init_state) {};
    const StateName& GetState() { return state_; };
    std::string GetStateString() const {
      switch(state_) {
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
    StateName state_;
};
