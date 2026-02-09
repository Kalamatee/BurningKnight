#ifndef BK_AROS_STATE_MANAGER_H
#define BK_AROS_STATE_MANAGER_H

typedef enum BKGameState {
    BK_GAME_STATE_RUNNING,
    BK_GAME_STATE_PAUSED,
    BK_GAME_STATE_GAME_OVER
} BKGameState;

typedef struct BKStateManager {
    BKGameState state;
    float run_time;
} BKStateManager;

void bk_state_manager_init(BKStateManager *manager);
void bk_state_manager_shutdown(BKStateManager *manager);
void bk_state_manager_toggle_pause(BKStateManager *manager);
int bk_state_manager_is_paused(const BKStateManager *manager);
void bk_state_manager_set_game_over(BKStateManager *manager);
float bk_state_manager_run_time(const BKStateManager *manager);
void bk_state_manager_update(BKStateManager *manager, float dt);

#endif
