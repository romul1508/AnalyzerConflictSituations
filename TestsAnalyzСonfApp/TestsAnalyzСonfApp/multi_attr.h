#include <math.h>

#ifndef _MULTI_
#define _MULTI_

#define EPS 0.000001
#define __COM__ 1 // Center Of Maximum, used only one pointed value for output

typedef struct points_t
{
	float		*pt;	// value of variable
	float		*mju;
	int 		number_pt;
}points_t;

typedef struct tri_term_t
{
	float		start;	//    __/\__ function, _start/center\finish_
	float		center;	// if start == center |\_
	float		finish; // if finish == center _/|
}tri_term_t;

typedef struct trap_term_t
{
	float 		start; 	// _/-\_ function _start/trap_s-trap_f\finish_
	float		trap_s; //
	float		trap_f;
	float		finish;
}trap_term_t;

// #define NULL            0
#define TRIANGLE_TYPE 	1
#define TRAP_TYPE		2
#define SIMPLE_TYPE		3
#define POINTS_TYPE		4

typedef struct eterm_t
{
	char 		type;
	char 		name[64];
	union
	{
		tri_term_t 	tri_term;
		trap_term_t	trap_term;
		points_t	points;
		float 		simple_point;
	};
}eterm_t;

typedef struct input_t
{
	char 		name_inp[64];
	eterm_t		*terms;
	int			num_terms;
}input_t;

typedef struct output_t
{
	char 		name_outp[64];
	eterm_t		*terms;
	int			num_terms;
}output_t;

typedef struct rule_t
{
	char 		name_rule[64];
	input_t		**inps;			// used inputs array of pointers
	int			*id_inp;
	int			*inp_id_term;
	int			num_inps;		// number of inputs in rule totally

	output_t	**outps; 		// only one structure, but store as pointer
	int			*id_outp;
	int			*outp_id_term;
	int			num_outps;		// term of output
}rule_t;

typedef struct fuzzy_block_t
{
	rule_t 		*rules;
	int			number_rules;
	input_t		*all_inp;
	float		*inputs,
				**m_inputs; // [num_inp][num_rule_for_each_input]
	int			num_inp;
	output_t	*all_outp;
	float		*outputs,
				**m_outputs;// [num_inp][num_rule_for_each_input]
	int			num_outp;
}fuzzy_block_t;

typedef struct 
{   
   int       index_comb_stup;  // индекс комбинации в общем списке
   float    price;            // показатель комбинации - цена (критерий)
   float    mod_otklon;       // показатель комбинации - модуль отклонения суммарной отключаемой мощности для комбинации ступеней (критерий)
   float    deltaOG_ON;       // разность ступенчатого управления по генерации и нагрузкам с непрерывным 
   float    target_func;      // значение целевой функции альтернативы
   float    fuzzy_value;      // значение расчитанное нечетким контроллером (в случае ошибки в многокритериальном анализе, берем лучшее значение отсюда)  
   bool      is_best;          // признак, что альтернатива лучшая и ее нужно выбрать
} TAlternative;

typedef TAlternative*	LPAlternative;


void sort_points_by_value( points_t *pts );
int read_data( fuzzy_block_t *blk, const char* FileName );
void print_controller( fuzzy_block_t *blk );
float calc_m_term( eterm_t *term, float value );
float calc_triangle( tri_term_t *term, float value );
float calc_trap( trap_term_t *term, float value );
float calc_simple( float simple, float value );
float calc_points( points_t *pts, float value );
float calc_m_term( eterm_t *term, float value );
void contr_fuzzy( fuzzy_block_t *blk );
void minDob(int count_alternatives, LPAlternative pAlternatives, int typeKr, double* d);
int calcTargetFuncTwoKrit(int count_alternatives, LPAlternative pAlternatives);
int calcTargetFuncThreeKrit(int count_alternatives, LPAlternative pAlternatives);

#endif
