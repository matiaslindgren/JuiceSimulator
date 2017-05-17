#include <stack>
#include <string>
#include "GameState.hpp"

class StateManager {
  public:
    explicit StateManager() {};
    void push_state(StateName state) { states.push(GameState(state)); };
    void pop_state() { states.pop(); };
    bool has_state() const { return !states.empty(); };
    const GameState& active_state() const { return states.top(); };
    std::string active_state_name() const { return active_state().get_state_string(); }
  private:
    std::stack<GameState> states;
};
