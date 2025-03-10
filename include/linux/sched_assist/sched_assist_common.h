/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (C) 2020 Oplus. All rights reserved.
 */

#ifndef _OPLUS_SCHED_COMMON_H_
#define _OPLUS_SCHED_COMMON_H_

#include <linux/version.h>

#define ux_err(fmt, ...) \
		printk_deferred(KERN_ERR "[SCHED_ASSIST_ERR][%s]"fmt, __func__, ##__VA_ARGS__)
#define ux_warn(fmt, ...) \
		printk_deferred(KERN_WARNING "[SCHED_ASSIST_WARN][%s]"fmt, __func__, ##__VA_ARGS__)
#define ux_debug(fmt, ...) \
		printk_deferred(KERN_INFO "[SCHED_ASSIST_INFO][%s]"fmt, __func__, ##__VA_ARGS__)

#define UX_MSG_LEN 64
#define UX_DEPTH_MAX 5

/* define for sched assist thread type, keep same as the define in java file */
#define SA_OPT_CLEAR     (0)
#define SA_TYPE_LIGHT    (1 << 0)
#define SA_TYPE_HEAVY    (1 << 1)
#define SA_TYPE_ANIMATOR (1 << 2)
#define SA_TYPE_LISTPICK (1 << 3)
#ifdef CONFIG_UCLAMP_TASK
#define SA_TYPE_TURBO    (1 << 4) /* for high load rt boost, used in audio*/
#endif
#ifdef CONFIG_KERNEL_LOCK_OPT
#define SA_TYPE_LISTPICK_LOCK (1 << 5)
#endif
#define SA_OPT_SET       (1 << 7)
#define SA_TYPE_INHERIT  (1 << 8)
#define SA_TYPE_ONCE_UX  (1 << 9)
#define SA_TYPE_ID_CAMERA_PROVIDER  (1 << 10)
#define SA_TYPE_ID_ALLOCATOR_SER    (1 << 11)



#define SCHED_ASSIST_UX_MASK (0xFF)

/* define for sched assist scene type, keep same as the define in java file */
#define SA_SCENE_OPT_CLEAR  (0)
#define SA_LAUNCH           (1 << 0)
#define SA_SLIDE            (1 << 1)
#define SA_CAMERA           (1 << 2)
#define SA_ANIM_START       (1 << 3) /* we care about both launcher and top app */
#define SA_ANIM             (1 << 4) /* we only care about launcher */
#define SA_INPUT            (1 << 5)
#define SA_LAUNCHER_SI      (1 << 6)
#define SA_SCENE_OPT_SET    (1 << 7)

#ifdef CONFIG_CGROUP_SCHED
#define SA_CGROUP_DEFAULT			(1)
#define SA_CGROUP_FOREGROUD			(2)
#define SA_CGROUP_BACKGROUD			(3)
#define SA_CGROUP_TOP_APP			(4)
#endif

/* define for load balance task type */
#define SA_HIGH_LOAD  	1
#define SA_LOW_LOAD  	0
#define SA_UX 	1
#define SA_TOP 	2
#define SA_FG 	3
#define SA_BG 	4

#ifdef CONFIG_OPLUS_FEATURE_SCHED_SPREAD
DECLARE_PER_CPU(struct task_count_rq, task_lb_count);
#endif

enum UX_STATE_TYPE
{
	UX_STATE_INVALID = 0,
	UX_STATE_NONE,
	UX_STATE_SCHED_ASSIST,
	UX_STATE_INHERIT,
	MAX_UX_STATE_TYPE,
};

enum INHERIT_UX_TYPE
{
	INHERIT_UX_BINDER = 0,
	INHERIT_UX_RWSEM,
	INHERIT_UX_MUTEX,
	INHERIT_UX_SEM,
	INHERIT_UX_FUTEX,
	INHERIT_UX_MAX,
};

struct ux_sched_cluster {
	struct cpumask cpus;
	unsigned long capacity;
};

struct ux_sched_cputopo {
	int cls_nr;
	struct ux_sched_cluster sched_cls[NR_CPUS];
};

#ifdef CONFIG_OPLUS_SYSTEM_KERNEL_QCOM
extern unsigned int walt_scale_demand_divisor;
#else
extern unsigned int walt_ravg_window;
#define walt_scale_demand_divisor  (walt_ravg_window >> SCHED_CAPACITY_SHIFT)
#endif
#define scale_demand(d) ((d)/walt_scale_demand_divisor)

#ifdef CONFIG_OPLUS_FEATURE_SCHED_SPREAD
struct task_count_rq{
	int ux_high;
	int ux_low;
	int top_high;
	int top_low;
	int foreground_high;
	int foreground_low;
	int background_high;
	int background_low;
};

enum OPLUS_LB_TYPE
{
	OPLUS_LB_UX = 1,
	OPLUS_LB_TOP,
	OPLUS_LB_FG,
	OPLUS_LB_BG,
	OPLUS_LB_MAX,
};
#endif

enum ANMATION_TYPE
{
	ANNIMATION_END = 0,
	APP_START_ANIMATION,
	APP_EXIT_ANIMATION,
	ULIMIT_PROCESS,
	LAUNCHER_SI_START,
	BACKUP,
	SYSTEMUI_SPLIT_STARTM,
};

struct rq;
extern unsigned int ux_uclamp_value;
extern int sysctl_input_boost_enabled;
extern int sysctl_animation_type;
extern int sysctl_input_boost_enabled;
extern int sysctl_sched_assist_ib_duration_coedecay;
extern int sched_assist_ib_duration_coedecay;
extern u64 sched_assist_input_boost_duration;
extern int ux_prefer_cpu[];
extern int sysctl_animation_type;

void ux_init_rq_data(struct rq *rq);
void ux_init_cpu_data(void);
#ifdef CONFIG_OPLUS_FEATURE_SCHED_SPREAD
void init_rq_cpu(int cpu);
#endif

bool test_list_pick_ux(struct task_struct *task);
void enqueue_ux_thread(struct rq *rq, struct task_struct *p);
void dequeue_ux_thread(struct rq *rq, struct task_struct *p);
void pick_ux_thread(struct rq *rq, struct task_struct **p, struct sched_entity **se);
extern bool sched_assist_pick_next_task_opt(struct cfs_rq *cfs_rq, struct sched_entity **se);
void inherit_ux_dequeue(struct task_struct *task, int type);
void inherit_ux_dequeue_refs(struct task_struct *task, int type, int value);
void inherit_ux_enqueue(struct task_struct *task, int type, int depth);
void inherit_ux_inc(struct task_struct *task, int type);
void inherit_ux_sub(struct task_struct *task, int type, int value);

void set_inherit_ux(struct task_struct *task, int type, int depth, int inherit_val);
void reset_inherit_ux(struct task_struct *inherit_task, struct task_struct *ux_task, int reset_type);
void unset_inherit_ux(struct task_struct *task, int type);
void unset_inherit_ux_value(struct task_struct *task, int type, int value);
void inc_inherit_ux_refs(struct task_struct *task, int type);

bool test_task_ux(struct task_struct *task);
bool test_task_ux_depth(int ux_depth);
bool test_inherit_ux(struct task_struct *task, int type);
bool test_set_inherit_ux(struct task_struct *task);
int get_ux_state_type(struct task_struct *task);

bool test_ux_task_cpu(int cpu);
bool test_ux_prefer_cpu(struct task_struct *task, int cpu);
void find_ux_task_cpu(struct task_struct *task, int *target_cpu);
void oplus_boost_kill_signal(int sig, struct task_struct *cur, struct task_struct *p);
static inline void find_slide_boost_task_cpu(struct task_struct *task, int *target_cpu) {}

void sched_assist_systrace_pid(pid_t pid, int val, const char *fmt, ...);
#define SA_SYSTRACE_MAGIC 123
#define sched_assist_systrace(...)  sched_assist_systrace_pid(SA_SYSTRACE_MAGIC, __VA_ARGS__)

void place_entity_adjust_ux_task(struct cfs_rq *cfs_rq, struct sched_entity *se, int initial);
bool should_ux_task_skip_further_check(struct sched_entity *se);
bool should_ux_preempt_wakeup(struct task_struct *wake_task, struct task_struct *curr_task);
bool should_ux_task_skip_cpu(struct task_struct *task, unsigned int cpu);
bool set_ux_task_to_prefer_cpu(struct task_struct *task, int *orig_target_cpu);
int set_ux_task_cpu_common_by_prio(struct task_struct *task, int *target_cpu, bool boost, bool prefer_idle, unsigned int type);
bool ux_skip_sync_wakeup(struct task_struct *task, int *sync);
void set_ux_task_to_prefer_cpu_v1(struct task_struct *task, int *orig_target_cpu, bool *cond);
bool im_mali(struct task_struct *p);
bool cgroup_check_set_sched_assist_boost(struct task_struct *p);
int get_st_group_id(struct task_struct *task);
void cgroup_set_sched_assist_boost_task(struct task_struct *p);
#ifdef CONFIG_OPLUS_FEATURE_SCHED_SPREAD
void inc_ld_stats(struct task_struct *tsk, struct rq *rq);
void dec_ld_stats(struct task_struct *tsk, struct rq *rq);
void update_load_flag(struct task_struct *tsk, struct rq *rq);

int task_lb_sched_type(struct task_struct *tsk);
unsigned long reweight_cgroup_task(u64 slice, struct sched_entity *se, unsigned long task_weight, struct load_weight *lw);
#if !defined(CONFIG_OPLUS_SYSTEM_KERNEL_QCOM) || (LINUX_VERSION_CODE < KERNEL_VERSION(5, 4, 0))
void sched_assist_spread_tasks(struct task_struct *p, cpumask_t new_allowed_cpus,
                        int start_cpu, int skip_cpu, cpumask_t *cpus, bool strict);
#else
void sched_assist_spread_tasks(struct task_struct *p, cpumask_t new_allowed_cpus,
                        int order_index, int end_index, int skip_cpu, cpumask_t *cpus, bool strict);
#endif
bool should_force_spread_tasks(void);
extern bool should_force_adjust_vruntime(struct sched_entity *se);
u64 sa_calc_delta(struct sched_entity *se, u64 delta_exec, unsigned long weight, struct load_weight *lw, bool calc_fair);
void update_rq_nr_imbalance(int cpu);
#endif

bool test_task_identify_ux(struct task_struct *task, int id_type_ux);
extern bool is_task_util_over(struct task_struct *task, int threshold);

#ifdef CONFIG_SCHED_WALT
bool sched_assist_task_misfit(struct task_struct *task, int cpu, int flag);
extern bool oplus_rt_new_idle_balance(struct rq *this_rq, u64 wallclock);
#else
inline bool sched_assist_task_misfit(struct task_struct *task, int cpu, int flag) { return false; }
static inline bool oplus_rt_new_idle_balance(struct rq *this_rq, u64 wallclock) { return false; }
#endif

#ifdef CONFIG_OPLUS_FEATURE_AUDIO_OPT
void update_sa_task_stats(struct task_struct *tsk, u64 delta_ns, int stats_type);
void sched_assist_im_systrace_c(struct task_struct *tsk, int tst_type);
void sched_assist_update_record(struct task_struct *p, u64 delta_ns, int stats_type);
bool sched_assist_pick_next_task(struct cfs_rq *cfs_rq, struct sched_entity *se);
#endif

extern bool skip_check_preempt_curr(struct rq *rq, struct task_struct *p, int flags);

static inline bool is_heavy_ux_task(struct task_struct *task)
{
	return task->ux_state & SA_TYPE_HEAVY;
}

static inline bool is_animation_ux(struct task_struct *task)
{
	return task->ux_state & SA_TYPE_ANIMATOR;
}
static inline void set_once_ux(struct task_struct *task)
{
	task->ux_state |= SA_TYPE_ONCE_UX;
}

static inline void set_heavy_ux(struct task_struct *task)
{
	task->ux_state |= SA_TYPE_HEAVY;
}

static inline bool sched_assist_scene(unsigned int scene)
{
	if (unlikely(!sysctl_sched_assist_enabled))
		return false;

	switch (scene) {
	case SA_LAUNCH:
		return sysctl_sched_assist_scene & SA_LAUNCH;
	case SA_SLIDE:
		return sysctl_slide_boost_enabled;
	case SA_INPUT:
		return sysctl_input_boost_enabled;
	case SA_CAMERA:
		return sysctl_sched_assist_scene & SA_CAMERA;
	case SA_LAUNCHER_SI:
		return sysctl_animation_type == LAUNCHER_SI_START;
	case SA_ANIM:
		return sysctl_sched_assist_scene & SA_ANIM;
	default:
		return sysctl_sched_assist_scene & scene;
	}
}

static inline bool is_sched_assist_scene(void) {
	return sched_assist_scene(SA_SLIDE) || sched_assist_scene(SA_INPUT) || sched_assist_scene(SA_LAUNCHER_SI) || sched_assist_scene(SA_ANIM);
}

static inline bool should_skip_detach_tasks(struct task_struct *tsk)
{
	return (sysctl_sched_assist_enabled >= 2) && test_task_ux(tsk) && !sched_assist_scene(SA_CAMERA);
}

#ifdef CONFIG_OPLUS_FEATURE_SCHED_SPREAD
static inline bool is_spread_task_enabled(void)
{
	return (sysctl_sched_assist_enabled >= 2) && !sched_assist_scene(SA_CAMERA);
}
#endif

static inline bool task_demand_ignore_wait_time(int event)
{
	return false;
}

bool should_limit_task_skip_cpu(struct task_struct *p, int cpu);
bool set_limit_task_target_cpu(struct task_struct *p, int *cpu);
bool is_sf(struct task_struct *p);

inline bool is_launcher(struct task_struct *p);
inline bool ux_debug_enable(void);
void ux_debug_systrace_c(int pid, int val);

int task_index_of_sf_union(struct task_struct *p);
void sf_task_util_record(struct task_struct *p);
u64 sf_union_ux_load(struct task_struct *tsk, u64 timeline);

#endif /* _OPLUS_SCHED_COMMON_H_ */
