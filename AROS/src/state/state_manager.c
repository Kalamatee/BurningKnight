#include "state/state_manager.h"

#include "util/log.h"

void bk_state_manager_init(BKStateManager *manager) {
    manager->state = BK_GAME_STATE_RUNNING;
    manager->run_time = 0.0f;
    bk_log_info("State manager initialized");
}

void bk_state_manager_shutdown(BKStateManager *manager) {
    (void)manager;
    bk_log_info("State manager shutdown");
}

void bk_state_manager_toggle_pause(BKStateManager *manager) {
    if (manager->state == BK_GAME_STATE_RUNNING) {
        manager->state = BK_GAME_STATE_PAUSED;
        bk_log_info("Game paused");
    } else if (manager->state == BK_GAME_STATE_PAUSED) {
        manager->state = BK_GAME_STATE_RUNNING;
        bk_log_info("Game resumed");
    }
}

int bk_state_manager_is_paused(const BKStateManager *manager) {
    return manager->state == BK_GAME_STATE_PAUSED;
}

void bk_state_manager_set_game_over(BKStateManager *manager) {
    if (manager->state != BK_GAME_STATE_GAME_OVER) {
        manager->state = BK_GAME_STATE_GAME_OVER;
        bk_log_info("Game over");
    }
}

float bk_state_manager_run_time(const BKStateManager *manager) {
    return manager->run_time;
}

void bk_state_manager_update(BKStateManager *manager, float dt) {
    if (manager->state == BK_GAME_STATE_RUNNING) {
        manager->run_time += dt;
    }
}
