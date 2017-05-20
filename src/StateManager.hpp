#include <stack>
#include <string>
#include "GameState.hpp"

class StateManager {
  public:
    explicit StateManager() {};
    void PushState(StateName state) { states_.push(GameState(state)); };
    void PopState() { states_.pop(); };
    bool HasState() const { return !states_.empty(); };
    const GameState& ActiveState() const { return states_.top(); };
    std::string ActiveStateName() const { return ActiveState().get_state_string(); }
  private:
    std::stack<GameState> states_;
};
