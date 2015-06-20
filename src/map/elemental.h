/****************************************************************************!
*                _           _   _   _                                       *
*               | |__  _ __ / \ | |_| |__   ___ _ __   __ _                  *
*               | '_ \| '__/ _ \| __| '_ \ / _ \ '_ \ / _` |                 *
*               | |_) | | / ___ \ |_| | | |  __/ | | | (_| |                 *
*               |_.__/|_|/_/   \_\__|_| |_|\___|_| |_|\__,_|                 *
*                                                                            *
******************************************************************************
*                          www.brathena.org                                  *
*****************************************************************************/

#ifndef MAP_ELEMENTAL_H
#define MAP_ELEMENTAL_H

#include "map/status.h" // struct status_data, struct status_change
#include "map/unit.h" // struct unit_data
#include "common/cbasetypes.h"
#include "common/mmo.h" // NAME_LENGTH

#include <stdarg.h>

/**
 * Defines
 **/
#define MIN_ELETHINKTIME 100
#define MIN_ELEDISTANCE 2
#define MAX_ELEDISTANCE 5
#define EL_MODE_AGGRESSIVE (MD_CANMOVE|MD_AGGRESSIVE|MD_CANATTACK)
#define EL_MODE_ASSIST (MD_CANMOVE|MD_ASSIST)
#define EL_MODE_PASSIVE MD_CANMOVE
#define EL_SKILLMODE_PASIVE 0x1
#define EL_SKILLMODE_ASSIST 0x2
#define EL_SKILLMODE_AGGRESSIVE 0x4

#define elemental_stop_walking(ed, type) (unit->stop_walking(&(ed)->bl, (type)))
#define elemental_stop_attack(ed)        (unit->stop_attack(&(ed)->bl))

/**
 * Structures
 **/
struct elemental_skill {
	unsigned short id, lv;
	short mode;
};

struct s_elemental_db {
	int class_;
	char sprite[NAME_LENGTH], name[NAME_LENGTH];
	unsigned short lv;
	short range2, range3;
	struct status_data status;
	struct view_data vd;
	struct elemental_skill skill[MAX_ELESKILLTREE];
};

struct elemental_data {
	struct block_list bl;
	struct unit_data ud;
	struct view_data *vd;
	struct status_data base_status, battle_status;
	struct status_change sc;
	struct regen_data regen;

	struct s_elemental_db *db;
	struct s_elemental elemental;

	struct map_session_data *master;
	int summon_timer;
	int skill_timer;

	int64 last_thinktime, last_linktime, last_spdrain_time;
	short min_chase;
	int target_id, attacked_id;
};

/**
 * elemental.c Interface
 **/
struct elemental_interface {

	/* vars */
	struct s_elemental_db db[MAX_ELEMENTAL_CLASS]; // Elemental Database

	/* */
	int (*init) (bool minimal);
	void (*final) (void);
	/* funcs */
	bool (*class) (int class_);
	struct view_data * (*get_viewdata) (int class_);

	int (*create) (struct map_session_data *sd, int class_, unsigned int lifetime);
	int (*data_received) (struct s_elemental *ele, bool flag);
	int (*save) (struct elemental_data *ed);

	int (*change_mode_ack) (struct elemental_data *ed, int mode);
	int (*change_mode) (struct elemental_data *ed, int mode);

	void (*heal) (struct elemental_data *ed, int hp, int sp);
	int (*dead) (struct elemental_data *ed);

	int (*delete) (struct elemental_data *ed, int reply);
	void (*summon_stop) (struct elemental_data *ed);

	int (*get_lifetime) (struct elemental_data *ed);

	int (*unlocktarget) (struct elemental_data *ed);
	int (*skillnotok) (uint16 skill_id, struct elemental_data *ed);
	int (*set_target) (struct map_session_data *sd, struct block_list *bl);
	int (*clean_single_effect) (struct elemental_data *ed, uint16 skill_id);
	int (*clean_effect) (struct elemental_data *ed);
	int (*action) (struct elemental_data *ed, struct block_list *bl, int64 tick);
	struct skill_condition (*skill_get_requirements) (uint16 skill_id, uint16 skill_lv);

	int (*read_skilldb) (void);
	void (*reload_db) (void);
	void (*reload_skilldb) (void);

	int (*search_index) (int class_);
	void (*summon_init) (struct elemental_data *ed);
	int (*summon_end_timer) (int tid, int64 tick, int id, intptr_t data);
	int (*ai_sub_timer_activesearch) (struct block_list *bl, va_list ap);
	int (*ai_sub_timer) (struct elemental_data *ed, struct map_session_data *sd, int64 tick);
	int (*ai_sub_foreachclient) (struct map_session_data *sd, va_list ap);
	int (*ai_timer) (int tid, int64 tick, int id, intptr_t data);
	int (*read_db) (void);
};

struct elemental_interface *elemental;

#ifdef BRATHENA_CORE
void elemental_defaults(void);
#endif // BRATHENA_CORE

#endif /* MAP_ELEMENTAL_H */
