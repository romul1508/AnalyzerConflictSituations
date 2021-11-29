#include <stdlib.h>
#include <stdio.h>
#include <string.h>
// #include <math.h>
#include "multi_attr.h"

//#include <sys/types.h>
//#include <sys/timeb.h>

// #define EPS 0.000001
// #define __COM__ 1 // Center Of Maximum, used only one pointed value for output

//typedef struct points_t
//{
//	float		*pt;	// value of variable
//	float		*mju;
//	int 		number_pt;
//}points_t;
// void sort_points_by_value( points_t *pts );
//typedef struct tri_term_t
//{
//	float		start;	//    __/\__ function, _start/center\finish_
//	float		center;	// if start == center |\_
//	float		finish; // if finish == center _/|
//}tri_term_t;

//typedef struct trap_term_t
//{
//	float 		start; 	// _/-\_ function _start/trap_s-trap_f\finish_
//	float		trap_s; //
//	float		trap_f;
//	float		finish;
//}trap_term_t;

//#define TRIANGLE_TYPE 	1
//#define TRAP_TYPE		2
//#define SIMPLE_TYPE		3
//#define POINTS_TYPE		4

//typedef struct eterm_t
//{
//	char 		type;
//	char 		name[64];
//	union
//	{
//		tri_term_t 	tri_term;
//		trap_term_t	trap_term;
//		points_t	points;
//		float 		simple_point;
//	};
//}eterm_t;

//typedef struct input_t
//{
//	char 		name_inp[64];
//	eterm_t		*terms;
//	int			num_terms;
//}input_t;

//typedef struct output_t
//{
//	char 		name_outp[64];
//	eterm_t		*terms;
//	int			num_terms;
//}output_t;

//typedef struct rule_t
//{
//	char 		name_rule[64];
//	input_t		**inps;			// used inputs array of pointers
//	int			*id_inp;
//	int			*inp_id_term;
//	int			num_inps;		// number of inputs in rule totally
//
//	output_t	**outps; 		// only one structure, but store as pointer
//	int			*id_outp;
//	int			*outp_id_term;
//	int			num_outps;		// term of output
//}rule_t;

//typedef struct fuzzy_block_t
//{
//	rule_t 		*rules;
//	int			number_rules;
//	input_t		*all_inp;
//	float		*inputs,
//				**m_inputs; // [num_inp][num_rule_for_each_input]
//	int			num_inp;
//	output_t	*all_outp;
//	float		*outputs,
//				**m_outputs;// [num_inp][num_rule_for_each_input]
//	int			num_outp;
//}fuzzy_block_t;

int Debug_ = 0;


int read_data( fuzzy_block_t *blk, const char* FileName )
{
	int 		i = 0,
				k = 0,
				m = 0,
				found = 0;
	int 		//end_pt = 0,
				start_pt = 0;
	char 		buf_str[256];
	char 		tmp_name[64],
				tmpc[256],
				tmpc2[256],
				tmpc3[256],
				tmpc4[256];
	input_t		*tmp_inp = NULL;
	output_t	*tmp_outp = NULL;
	eterm_t		*tmp_term = NULL;
	rule_t		*tmp_rule = NULL;
	FILE 		*fp=NULL;
	// fp = fopen( "data.fuz", "r" );
	fp = fopen( FileName, "r" );

	while( fgets( &buf_str[0], sizeof(buf_str), fp) != NULL) // || buf_str[0] != '\n')
	{
		if( buf_str[0] == '\r' || buf_str[0] == '\n' || buf_str[0] == '\0' || buf_str[0] == '#' )
		{ continue; }
		sscanf( &buf_str[0], "%s", &tmp_name[0] );
		if( strncmp(&tmp_name[0], "input:", 6) == 0 )
		{
			if( strncmp( &tmp_name[6], "term:", 5 ) == 0 )
			{
				i = blk->num_inp-1;
				tmp_inp = (blk->all_inp+i);
				tmp_inp->num_terms++;
				i = tmp_inp->num_terms;
				// pSubordinate_level_set = (unsigned int*) calloc (k, sizeof(unsigned int));
				// tmp_inp->terms = realloc( tmp_inp->terms, sizeof(eterm_t)*i );
				tmp_inp->terms = (eterm_t*)realloc( tmp_inp->terms, sizeof(eterm_t)*i );
				// 
				i--;
				tmp_term = (tmp_inp->terms+i);
				memset( tmp_term, 0, sizeof(eterm_t) );
				sscanf( &buf_str[0], "%s %s", &tmpc[0], &tmpc2[0] );
				if( strcmp(&tmpc2[0], "triangle") == 0 )
				{
					tmp_term->type = TRIANGLE_TYPE;
					sscanf( &buf_str[0], "%s %s %s %f %f %f",
							&tmpc[0], &tmpc2[0], &tmp_term->name[0],
							&tmp_term->tri_term.start,
							&tmp_term->tri_term.center,
							&tmp_term->tri_term.finish );
				}
				else if( strcmp( &tmpc2[0], "trap" ) == 0 )
				{
					tmp_term->type = TRAP_TYPE;
					sscanf( &buf_str[0], "%s %s %s %f %f %f %f",
							&tmpc[0], &tmpc2[0], &tmp_term->name[0],
							&tmp_term->trap_term.start,
							&tmp_term->trap_term.trap_s,
							&tmp_term->trap_term.trap_f,
							&tmp_term->trap_term.finish	);
				}
				else if( strcmp( &tmpc2[0], "simple_point" ) == 0 )
				{
					tmp_term->type = SIMPLE_TYPE;
					sscanf( &buf_str[0], "%s %s %s %f",
							&tmpc[0], &tmpc2[0], &tmp_term->name[0],
							&tmp_term->simple_point );
				}
				else if( strcmp( &tmpc2[0], "points" ) == 0 )
				{
					tmp_term->type = POINTS_TYPE;
					memset( &tmp_term->points, 0, sizeof(points_t));
					start_pt = 0;
					sscanf( &buf_str[0], "%s %s %s", &tmpc[0], &tmpc2[0], &tmp_term->name[0] );
					while( fgets( &buf_str[0], sizeof(buf_str), fp) != NULL )
					{
						if( buf_str[0] == '\0' || buf_str[0] == '\r' || buf_str[0] == '\n' || buf_str[0] == '#' )
							continue;
						if( strncmp( &buf_str[0], "start", 5 ) == 0 )
						{ start_pt = 1; }
						else if( strncmp( &buf_str[0], "end", 3 ) == 0 )
						{ start_pt = 0; break; }
						else if( start_pt )
						{
							tmp_term->points.number_pt++;
							tmp_term->points.pt = (float*)realloc( tmp_term->points.pt, sizeof(int)*tmp_term->points.number_pt );
							tmp_term->points.mju = (float*)realloc( tmp_term->points.mju, sizeof(int)*tmp_term->points.number_pt );
							i = tmp_term->points.number_pt-1;
							sscanf( &buf_str[0], "%f %f", (tmp_term->points.pt+i), (tmp_term->points.mju+i) );
						}
						if( feof(fp) != 0 )
						{
							printf( "bad file format!!! error reading points!"); // %d %s\n", __LINE__, __func__ );
							exit(-1);
							break;
						}
					}
					if( feof(fp) != 0 )
					{
						printf( "bad file format!!! error reading points!"); // %d %s\n", __LINE__, __func__ );
						exit(-1);
						break;
					}
					sort_points_by_value( &tmp_term->points );
				}
			}
			else if( strncmp( &tmp_name[6], "name:", 5 ) == 0 )
			{
				blk->num_inp++;
				blk->all_inp = (input_t*)realloc( blk->all_inp, sizeof(input_t)*blk->num_inp );
				i = blk->num_inp-1;
				tmp_inp = (blk->all_inp+i);
				memset( tmp_inp, 0, sizeof(input_t) );
				sscanf( &buf_str[0], "%s %s", &tmpc[0], &tmp_inp->name_inp[0] );
			}
		}
		else if( strncmp(&tmp_name[0], "output:", 7) == 0 )
		{
			if( strncmp( &tmp_name[7], "term:", 5 ) == 0 )
			{
				i = blk->num_outp-1;
				tmp_outp = (blk->all_outp+i);
				tmp_outp->num_terms++;
				i = tmp_outp->num_terms;
				tmp_outp->terms = (eterm_t*)realloc( tmp_outp->terms, sizeof(eterm_t)*i );
				i--;
				tmp_term = (tmp_outp->terms+i);
				memset( tmp_term, 0, sizeof(eterm_t) );
				sscanf( &buf_str[0], "%s %s", &tmpc[0], &tmpc2[0] );
				if( strcmp( &tmpc2[0], "simple_point" ) == 0 )
				{
					tmp_term->type = SIMPLE_TYPE;
					sscanf( &buf_str[0], "%s %s %s %f",
							&tmpc[0], &tmpc2[0], &tmp_term->name[0],
							&tmp_term->simple_point );
				}
				else
				{
					printf("only 'simple_point' type is supported for outputs yet\n");
				}
			}
			else if( strncmp( &tmp_name[7], "name:", 5 ) == 0 )
			{
				blk->num_outp++;
				blk->all_outp = (output_t*)realloc( blk->all_outp, sizeof(input_t)*blk->num_outp );
				i = blk->num_outp-1;
				tmp_outp = (blk->all_outp+i);
				memset( tmp_outp, 0, sizeof(output_t) );
				sscanf( &buf_str[0], "%s %s", &tmpc[0], &tmp_outp->name_outp[0] );
			}
		}
		else if( strncmp(&tmp_name[0], "rule:", 7) == 0 )
		{
			found = 0;
			// rule: rule_1 input input1 term1
			sscanf( &buf_str[0], "%s %s %s %s %s", &tmpc[0], &tmp_name[0], &tmpc2[0], &tmpc3[0], &tmpc4[0] );
			for( i=0; i < blk->number_rules; ++i )
			{
				if( strcmp( &blk->rules[i].name_rule[0], &tmp_name[0] ) == 0 )
				{
					// found existing rule & update it
					found = 1;
					break;
				}
			}
			if( found )
			{ tmp_rule = (blk->rules+i); }
			else
			{
				blk->number_rules++;
				blk->rules = (rule_t*)realloc( blk->rules, sizeof(rule_t)*blk->number_rules );
				tmp_rule = (blk->rules+(blk->number_rules-1));
				memset( tmp_rule, 0, sizeof(rule_t) );
				strcpy( &tmp_rule->name_rule[0], &tmp_name[0] );
			}
			if( strcmp(&tmpc2[0], "input") == 0 )
			{
				found = 0;
				for( i=0; i < blk->num_inp; ++i )
				{
					if( strcmp( &blk->all_inp[i].name_inp[0], &tmpc3[0] ) == 0 )
					{
						found = 1;
						tmp_inp = &blk->all_inp[i];
						break;
					}
				}
				if( !found )
				{
					printf( "can't find input with name %s\n", &tmpc3[0] );
					continue;
				}
				found = 0;
				for( m=0; m < tmp_rule->num_inps; ++m )
				{
					if( tmp_rule->inps[m] == tmp_inp )
					{
						found = 1;
						break;
					}
				}
				if( !found )
				{
					tmp_rule->num_inps++;
					i = tmp_rule->num_inps-1;
					tmp_rule->inps = (input_t**)realloc( tmp_rule->inps, sizeof(int)*tmp_rule->num_inps );
					tmp_rule->id_inp = (int*)realloc( tmp_rule->id_inp, sizeof(int)*tmp_rule->num_inps );
					tmp_rule->inps[i] = tmp_inp;
					tmp_rule->id_inp[i] = m;
					tmp_rule->inp_id_term = (int*)realloc( tmp_rule->inp_id_term, sizeof(int)*tmp_rule->num_inps );
					tmp_rule->inp_id_term[i] = -1; // index initializing with -1
				}
				else
				{
					printf( "can't assign input <%s> more one time to the same rule!\n", &tmp_inp->name_inp[0] );
				}

				found = 0;
				for( m=0; m < tmp_inp->num_terms; ++m )
				{
					if( strcmp( &tmp_inp->terms[m].name[0], &tmpc4[0] ) == 0 )
					{ found = 1; break; }
				}
				if( !found )
				{
					printf(" term not found term=\n");//, __func__, __LINE__, &tmpc4[0] );
					continue;
				}
				tmp_rule->inp_id_term[i] = m;
			}
			else if( strcmp(&tmpc2[0], "output") == 0 )
			{
				found = 0;
				for( i=0; i < blk->num_outp; ++i )
				{
					if( strcmp( &blk->all_outp[i].name_outp[0], &tmpc3[0] ) == 0 )
					{
						found = 1;
						tmp_outp = &blk->all_outp[i];
						break;
					}
				}
				if( !found )
				{
					printf( "can't find output with name %s\n", &tmpc3[0] );
					continue;
				}
				found = 0;
				for( m=0; m < tmp_rule->num_outps; ++m )
				{
					if( tmp_rule->outps[m] == tmp_outp )
					{
						found = 1;
						break;
					}
				}
				if( found )
				{
					printf( "can't use more than one term for output <%s> in the same rule!!!", &tmpc3[0] );
					continue;
				}

				found = 0;
				for( k=0; k < tmp_outp->num_terms; ++k )
				{
					if( strcmp( &tmp_outp->terms[k].name[0], &tmpc4[0] ) == 0 )
					{
						found = 1;
						break;
					}
				}
				if( !found )
				{
					printf( "Can't find term <%s> for output <%s>!\n", &tmpc4[0], &tmp_outp->name_outp[0] );
				}
				tmp_rule->num_outps++;
				tmp_rule->outps = (output_t**)realloc( tmp_rule->outps, sizeof(int)*tmp_rule->num_outps );
				tmp_rule->id_outp = (int*)realloc( tmp_rule->id_outp, sizeof(int)*tmp_rule->num_outps );
				i = tmp_rule->num_outps-1;
				tmp_rule->outps[i] = tmp_outp;
				tmp_rule->id_outp[i] = m;
				tmp_rule->outp_id_term = (int*)realloc( tmp_rule->outp_id_term, sizeof(int)*tmp_rule->num_outps );
				tmp_rule->outp_id_term[i] = k;
			}
			else
			{
				printf("some bad value for rule: input/output, got <%s>\n", &tmpc2[0] );
			}
		}
	}

	if( blk->inputs == NULL )
		blk->inputs = (float*)calloc( blk->num_inp, sizeof(float) );
	if( blk->outputs == NULL )
		blk->outputs = (float*)calloc( blk->num_outp, sizeof(float) );
	if( blk->m_inputs == NULL )
	{
		blk->m_inputs = (float**)calloc( blk->num_inp, sizeof(int) );
		for( i=0; i < blk->num_inp; ++i )
		{
			blk->m_inputs[i] = (float*)calloc( (blk->all_inp+i)->num_terms, sizeof(float) );
		}
	}
	if( blk->m_outputs == NULL )
	{
		blk->m_outputs = (float**)calloc( blk->num_outp, sizeof(int) );
		for( i=0; i < blk->num_outp; ++i )
		{
			blk->m_outputs[i] = (float*)calloc( (blk->all_outp+i)->num_terms, sizeof(float) );
		}
	}

	return 0;
}

void print_controller( fuzzy_block_t *blk )
{
	int i=0,
		k=0,
		z=0;
	printf("inputs--->\n");
	for(i=0; i < blk->num_inp; ++i )
	{
		printf( "input\t%s\n", &blk->all_inp[i].name_inp[0] );
		for( k=0; k < blk->all_inp[i].num_terms; ++k )
		{
			switch( ((blk->all_inp+i)->terms+k)->type )
			{
				case TRIANGLE_TYPE:
					printf( "\t\t%s __ %.4f // %.4f \\\\ %.4f __\n",
										blk->all_inp[i].terms[k].name,
										blk->all_inp[i].terms[k].tri_term.start,
										blk->all_inp[i].terms[k].tri_term.center,
										blk->all_inp[i].terms[k].tri_term.finish );
					break;
				case TRAP_TYPE:
					printf( "\t\t%s __ %.4f // %.4f -- %.4f \\\\ %.4f __\n",
										blk->all_inp[i].terms[k].name,
										blk->all_inp[i].terms[k].trap_term.start,
										blk->all_inp[i].terms[k].trap_term.trap_s,
										blk->all_inp[i].terms[k].trap_term.trap_f,
										blk->all_inp[i].terms[k].trap_term.finish );
					break;
				case SIMPLE_TYPE:
					printf( "\t\t%s | %.4f\n",
										blk->all_inp[i].terms[k].name,
										blk->all_inp[i].terms[k].simple_point);
					break;
				case POINTS_TYPE:
					printf( "\t\t%s\n", blk->all_inp[i].terms[k].name );
					for( z=0; z < blk->all_inp[i].terms[k].points.number_pt; ++z )
					{
						printf( "\t\t\t%.4f\t%.4f\n",
								blk->all_inp[i].terms[k].points.pt[z],
								blk->all_inp[i].terms[k].points.mju[z]);
					}
					break;
				default:
					printf( "some bad value of type for term of input %s\n", &(blk->all_inp+i)->name_inp[0] );
			}

		}
	}
	printf("outputs--->\n");
	for( i=0; i < blk->num_outp; ++i )
	{
		printf( "output\t%s\n", &blk->all_outp[i].name_outp[0] );
		for( k=0; k < blk->all_outp[i].num_terms; ++k )
		{
			if( blk->all_outp[i].terms[k].type == SIMPLE_TYPE )
			{
				printf( "\t\t%s |%.4f \n",
						blk->all_outp[i].terms[k].name,
						blk->all_outp[i].terms[k].simple_point );
			}
		}
	}
	printf("rules--->\n");
	for( i=0; i < blk->number_rules; ++i )
	{
		printf( "rule\t%s\n", &blk->rules[i].name_rule[0] );
		printf( "IF\n" );
		for( k=0; k < blk->rules[i].num_inps; ++k )
		{
			printf("(inp)%s is %s", &blk->rules[i].inps[k]->name_inp[0],
						&blk->rules[i].inps[k]->terms[ blk->rules[i].inp_id_term[k] ].name[0] );
			if( k != blk->rules[i].num_inps-1 )
				printf("AND\n");
			else
				printf("\n");
		}
		printf( "THEN\n" );
		for( k=0; k < blk->rules[i].num_outps; ++k )
		{
			printf("(outp)%s is %s", &blk->rules[i].outps[k]->name_outp[0],
						&blk->rules[i].outps[k]->terms[ blk->rules[i].outp_id_term[k] ].name[0] );
			if( k != blk->rules[i].num_outps-1 )
				printf("AND\n");
			else
				printf("\n");
		}
		printf("END\n\n");
	}
	return;
}

// float calc_m_term( eterm_t *term, float value );
// COGS Center of Gravity for Singletons
void contr_fuzzy( fuzzy_block_t *blk )
{
	int 	i = 0,
			k = 0,
			m = 0,
			z = 0;
	float 	ftmp=0.0,
			sum_f = 0.0,
			mult_f = 0.0,
			min_f = 0.0,
			sum_mf = 0.0;
	int 	found = 0;
	rule_t	*tmp_rule;
	output_t *tmp_outp;

	for( i=0; i < blk->num_inp; ++i )
		memset( blk->m_inputs[i], 0, sizeof(float)*(blk->all_inp+i)->num_terms );
	for( i=0; i < blk->num_outp; ++i )
		memset( blk->m_outputs[i], 0, sizeof(float)*(blk->all_outp+i)->num_terms );

	/* fuzzyfication */
	if( Debug_ )printf( "fuzzyfication-------->\n" );
	for( i=0; i < blk->num_inp; ++i )
	{
		for( k=0; k < (blk->all_inp+i)->num_terms; ++k )
		{
			ftmp = blk->inputs[i];
			blk->m_inputs[i][k] = calc_m_term( ((blk->all_inp+i)->terms+k), ftmp );
			if( blk->m_inputs[i][k] < 0 )
				blk->m_inputs[i][k] = 0.0;
			if( Debug_ )printf( "%.2f\t", blk->m_inputs[i][k] );
		}
		if( Debug_ )printf("\n arg=%f\n", ftmp);
	}
	/* defuzzyfication of input variables */
#ifdef __COM__
	/*for( i=0; i < blk->number_rules; ++i )
	{
		ftmp = 0.0;
		sum_f = 0.0;
		sum_mf = 0.0;
		mult_f = 1.0;
		min_f = 1.0;
		tmp_rule = (blk->rules+i);
		for( k = 0; k < tmp_rule->num_inps; ++k )
		{
			for( m=0; m < blk->num_inp; ++m )
			{
				if( (blk->all_inp+m) == tmp_rule->inps[k] )
				{break;}
			}
			ftmp = blk->m_inputs[m][ tmp_rule->inp_id_term[k] ];
			sum_mf += ftmp;
			mult_f = mult_f*ftmp;
			if( ftmp < min_f )
				min_f = ftmp; // min activation
		}
		if( mult_f > 0 ) // rule is active (agregation)
		{
			//sum_f = tmp_rule->outps

		}
	}*/
	for( i=0; i < blk->num_outp; ++i )
	{
		tmp_outp = (blk->all_outp+i);
		ftmp = 0.0;
		sum_mf = 0.0;
		sum_f = 0;
		for( k=0; k < blk->number_rules; ++k )
		{
			tmp_rule = (blk->rules+k);
			found = 0;
			for( m=0; m < tmp_rule->num_outps; ++m )
			{
				if( tmp_rule->id_outp[m] == i )
				{
					// found output in rule
					found = 1;
					break;
				}
			}
			if( !found )
				continue; // next rule, with dependence for this output
			//
			mult_f = 1.0;
			min_f = 1.0;
			for( z = 0; z < tmp_rule->num_inps; ++z )
			{
				ftmp = blk->m_inputs[ tmp_rule->id_inp[z] ][tmp_rule->inp_id_term[z]];
				//sum_f += tmp_outp->terms[tmp_rule->outp_id_term[m]].center * ftmp;
				//sum_mf += ftmp;
				if( ftmp < min_f )
					min_f = ftmp;
				mult_f = mult_f * min_f;
			}
			if( mult_f > 0.0 ) // rule is activised
			{
				blk->m_outputs[i][tmp_rule->outp_id_term[m]] += min_f;
			}
		}
		for( z=0; z < tmp_outp->num_terms; ++z )
		{
			ftmp = blk->m_outputs[i][z];
			if( (tmp_outp->terms+z)->type == SIMPLE_TYPE )
			{
				sum_f += (tmp_outp->terms+z)->simple_point * ftmp;
				sum_mf += ftmp;
			}
		}
		blk->outputs[i] = sum_f / sum_mf;
	}
#else

#endif

	return;
}

float calc_triangle( tri_term_t *term, float value )
{
	float k,b;
	float f_tmp;
	if( term->center == term->start )
	{
		if( value >= term->center )
		{
			if( value >= term->finish )
			{ return 0.0; }
			else
			{
				f_tmp = term->center - term->finish;
				k = 1.0 / f_tmp;
				b = -k * term->finish;
				return (k*value+b);
			}
		}
		else
		{ return 0.0; }
	}
	else if( term->center == term->finish )
	{
		if( value > term->center )
		{ return 0.0; }
		else
		{
			if( value <= term->start )
			{ return 0.0; }
			else
			{
				f_tmp = term->start - term->center;
				k = 1.0 / f_tmp;
				b = -k * term->start;
				return -(k*value+b);
			}
		}
	}
	else
	{
		if( value >= term->center )
		{
			if( value >= term->finish )
			{ return 0.0; }
			else
			{
				f_tmp = term->center - term->finish;
				k = 1.0 / f_tmp;
				b = -k * term->finish;
				return (k*value+b);
			}
		}
		else //if( value < term->center )
		{
			if( value <= term->start )
			{ return 0.0; }
			else
			{
				f_tmp = term->start - term->center;
				k = 1.0 / f_tmp;
				b = -k * term->start;
				return -(k*value+b);
			}
		}
	}
	return 0.0;
}

float calc_trap( trap_term_t *term, float value )
{
	float 	ftmp,k,b;
	int 	i;

	if( value >= term->trap_s && value <= term->trap_f )
	{ return 1.0; }
	else
	{
		if( value < term->start )
		{ return 0.0; }
		else if( value > term->finish )
		{ return 0.0; }
		else if( value < term->trap_s )
		{
			ftmp = term->start - term->trap_s;
			k = 1.0 / ftmp;
			b = -k * term->start;
			return -(k*value+b);
		}
		else if( value > term->trap_f )
		{
			ftmp = term->trap_f - term->finish;
			k = 1.0 / ftmp;
			b = -k * term->finish;
			return (k*value+b);
		}
	}
	return 0.0;
}

float calc_simple( float simple, float value )
{
	if( value - simple < EPS && value - simple > 0 )
		return 1.0;
	else if( simple - value < EPS && simple - value > 0 )
		return 1.0;
	else
		return 0.0;
}

void sort_points_by_value( points_t *pts )
{
	float 	tmp_pt,
			tmp_mju;
	int i,k;
	for( i=0; i < pts->number_pt; ++i )
	{
		tmp_pt = 0.0;
		tmp_mju = 0.0;
		for( k=1; k < pts->number_pt; ++k )
		{
			if( pts->pt[k] <  pts->pt[k-1] )
			{
				tmp_pt = pts->pt[k];
				pts->pt[k] = pts->pt[k-1];
				pts->pt[k-1] = tmp_pt;

				tmp_mju = pts->mju[k];
				pts->mju[k] = pts->mju[k-1];
				pts->mju[k-1] = tmp_mju;
			}
		}
	}
	return;
}

float calc_points( points_t *pts, float value )
{
	float ftmp, ftmp2, k, b;
	int i;
	for( i=1; i < pts->number_pt; ++i )
	{
		if( value <= pts->pt[i] && value >= pts->pt[i-1] )
		{
			/*ftmp = pts->pt[i-1] - pts->pt[i];
			ftmp2 = pts->mju[i] - pts->mju[i-1];
			k = ftmp2 / ftmp;
			b = -k * pts->pt[i-1];
			ftmp = -(k*value+b);
			if( ftmp < 0 )
				ftmp += 1;
			return ftmp;*/
			ftmp2 = pts->mju[i] - pts->mju[i-1];
			if( ftmp2 < 0 )
			{
				ftmp2 = -ftmp2;
				ftmp = pts->pt[i-1] - pts->pt[i];
				k = ftmp2 / ftmp;
				b = -k * pts->pt[i];
				return (k*value+b);
			}
			else
			{
				ftmp = pts->pt[i-1] - pts->pt[i];
				k = ftmp2 / ftmp;
				b = -k * pts->pt[i-1];
				return -(k*value+b);
			}
		}
	}
	return 0.0;
}

float calc_m_term( eterm_t *term, float value )
{
	switch( term->type )
	{
		case TRIANGLE_TYPE:
			return calc_triangle( &term->tri_term, value );
			break;
		case TRAP_TYPE:
			return calc_trap( &term->trap_term, value );
			break;
		case SIMPLE_TYPE:
			return calc_simple( term->simple_point, value );
			break;
		case POINTS_TYPE:
			return calc_points( &term->points, value );
			break;
		default:
			// printf( "%s some bad type(%d)!!! ret=0.0\n", __func__, term->type );
			printf( "some bad type!!! ret=0.0\n");//, __func__, term->type );
	}

	return 0.0;
}

//int main( int argc, char *argv[] )
//{
//	int 			i=0,
//					k=0,
//					m=0,
//					z=0;
//	// struct timeb	tm01;
//	//printf("Welcome to the QNX Momentics IDE\n");
//	fuzzy_block_t 	*blk;
//	blk = (fuzzy_block_t*)calloc( 1, sizeof(fuzzy_block_t) );
//	read_data( blk );
//	print_controller( blk );
//
//	// ftime( &tm01 );
//	// printf( "start time is %d sec %d msec\n", tm01.time, tm01.millitm );
//	
//	// ftime( &tm01 );
//	// printf( "end time is %d sec %d msec\n", tm01.time, tm01.millitm );
//	blk->inputs[0] = 0.00334375;
//	blk->inputs[1] = 0.438769;
//	blk->inputs[2] = 0.74274;
//	blk->inputs[3] = 0.2096;
//	// blk->outputs[0] = 0.25417;
//	
//
//	// blk->inputs[0] = 0.00334375;
//	// blk->inputs[1] = 0.2323;
//	// blk->inputs[2] = 0.0216;
//	// blk->inputs[3] = 0.2126;
//	// blk->outputs[0] = 1;
//    
//	contr_fuzzy( blk );
//	printf( "output=%f\t", blk->outputs[0] );
//
//
//
//	for( i=0; i < blk->num_outp; ++i )
//	{
//		free(blk->m_outputs[i]);
//	}
//
//	free(blk->m_outputs);
//
//	for( i=0; i < blk->num_inp; ++i )
//	{
//		free(blk->m_inputs[i]);
//	}
//	free(blk->m_inputs);
//	free(blk->outputs);
//	free(blk->inputs);
//	free(blk);
//
//	return EXIT_SUCCESS;
//}


#pragma region функция calcTargetFunc

// Производит расчет целевых функций при двух критериев
// и заполняет структуру LPAlternative pAlternatives
// возвращает код ошибки err - 0, если функция выполнена успешно 

int calcTargetFuncTwoKrit(int count_alternatives, LPAlternative pAlternatives){	
	int i, j, err, nKrit, iMin, count_fact_alt;
	double Den1, min;
	double *c1, *c2, *d, *W, *Den, *ZI, *F;
	double *Z[2];

	err = 0;	
	nKrit = 2;

	//Заполним нормированную матрицу решений
	
	c1 = (double*) calloc (count_alternatives, sizeof(double));
	c2 = (double*) calloc (count_alternatives, sizeof(double));
					
	minDob(count_alternatives, pAlternatives, 1, c1);    
	minDob(count_alternatives, pAlternatives, 2, c2);
    
	   
	//fprintf(file, "normirovan matrix solution loaded sucsessful\n");

	// рассчитаем веса критериев
    Den1 = 0;
    	
	W = (double*) calloc (nKrit, sizeof(double));
    Den = (double*) calloc (nKrit, sizeof(double));
	ZI = (double*) calloc (nKrit, sizeof(double));

	F = (double*) calloc (count_alternatives, sizeof(double));	
	
	for(int i=0; i<nKrit; i++)
	   Z[i] = (double*) calloc (count_alternatives, sizeof(double));

    for(int i = 0; i<nKrit; i++)
       ZI[i] = 0;


    // cначала для цены
	for(int i=0; i<count_alternatives; i++){
           Z[0][i] = c1[i];//response->normMatrixSol.krTypes[j].fields[i].value;
           ZI[0] = ZI[0] + 1 - Z[0][i];
    }

	if(ZI[0]==0)
    {
         //String Err = "Значения одного и того же критерия во всех альтернативах не могут быть одинаковы";
         //System.out.println("Values of the same criteria in all alternatives cannot be identical.");   
         err = -1;
         goto labelErrCalc2;
    }

	Den[0] = 1/(ZI[0] * ZI[0]);
    Den1 += Den[0];

	// теперь для модуля отклонения
	for(i=0; i<count_alternatives; i++){
           Z[1][i] = c2[i];//response->normMatrixSol.krTypes[j].fields[i].value;
           ZI[1] = ZI[1] + 1 - Z[1][i];
    }

	if(ZI[1]==0)
    {
         //String Err = "Значения одного и того же критерия во всех альтернативах не могут быть одинаковы";
         //System.out.println("Values of the same criteria in all alternatives cannot be identical.");       
         err = -1;
         goto labelErrCalc2;
    }

	Den[1] = 1/(ZI[1] * ZI[1]);
    Den1 += Den[1];

    /////////////////////////
    for (j = 0; j < nKrit; j++)
       W[j] = Den[j] / Den1;	   
	
    
	//fprintf(file, "calculated weights criterion\n");
	
    for (i = 0; i < count_alternatives; i++)
        F[i] = 0;
    for (i = 0; i < count_alternatives; i++) {
       double fKr[2];
       // для цены
       fKr[0] = (1 - c1[i]) * W[0];
       F[i] = F[i] + fKr[0];

	   // для модуля отклонения
	   fKr[1] = (1 - c2[i]) * W[1];
       F[i] = F[i] + fKr[1];

	   pAlternatives[i].target_func = F[i];
    }
    
	//fprintf(file, "calculated criteria function\n");

	// найдем альтернативу у которой величина целевой функции имеет минимальное значение
	// и установим признак isBest в состояние true
	iMin = -1;
	min = 1000000;	
	count_fact_alt = count_alternatives - 1;

    for (j = 0; j < count_fact_alt; j++) 
	{
       if (pAlternatives[j].target_func < min)
       {
		   min  = pAlternatives[j].target_func;     
		   iMin = j;
	   }
    }

	pAlternatives[iMin].is_best = true;

	/*for(i=0; i<count_alternatives; i++){
		if(fabs(pAlternatives[i].target_func - min) < 0.0001)
			pAlternatives[i].is_best = true;
		break;
	}*/

	if(iMin == -1)
		err = -1;

labelErrCalc2:
    // Освобождаем занятые ресурсы и возвращаяем результат выполнения функции
    free(c1);
    free(c2);
					
	free(W);
	free(Den);
	free(ZI);

	free(F);

    for(int i=0; i<nKrit; i++)
	    free(Z[i]);

	//fclose(file);
	return err;   
}

// Производит расчет целевых функций для трех критериев
// и заполняет структуру LPAlternative pAlternatives
// возвращает код ошибки err - 0, если функция выполнена успешно 

int calcTargetFuncThreeKrit(int count_alternatives, LPAlternative pAlternatives){
	int i, j, err, nKrit, iMin, count_fact_alt;
	double Den1, min;
	double *c1, *c2, *c3, *d, *W, *Den, *ZI, *F;
	double *Z[2];

	err = 0;	
	nKrit = 3;

	//Заполним нормированную матрицу решений
	
	c1 = (double*) calloc (count_alternatives, sizeof(double));
	c2 = (double*) calloc (count_alternatives, sizeof(double));
	c3 = (double*) calloc (count_alternatives, sizeof(double));
					
	minDob(count_alternatives, pAlternatives, 1, c1);    
	minDob(count_alternatives, pAlternatives, 2, c2);
	minDob(count_alternatives, pAlternatives, 0, c3);
    
	   
	//fprintf(file, "normirovan matrix solution loaded sucsessful\n");

	// рассчитаем веса критериев
    Den1 = 0;
    	
	W = (double*) calloc (nKrit, sizeof(double));
    Den = (double*) calloc (nKrit, sizeof(double));
	ZI = (double*) calloc (nKrit, sizeof(double));

	F = (double*) calloc (count_alternatives, sizeof(double));	
	
	for(int i=0; i<nKrit; i++)
	   Z[i] = (double*) calloc (count_alternatives, sizeof(double));

    for(int i = 0; i<nKrit; i++)
       ZI[i] = 0;


    // cначала для цены
	for(int i=0; i<count_alternatives; i++){
           Z[0][i] = c1[i];//response->normMatrixSol.krTypes[j].fields[i].value;
           ZI[0] = ZI[0] + 1 - Z[0][i];
    }

	if(ZI[0]==0)
    {
         //String Err = "Значения одного и того же критерия во всех альтернативах не могут быть одинаковы";
         //System.out.println("Values of the same criteria in all alternatives cannot be identical.");   
         err = -1;
         goto labelErrCalc2;
    }

	Den[0] = 1/(ZI[0] * ZI[0]);
    Den1 += Den[0];

	// теперь для модуля отклонения
	for(i=0; i<count_alternatives; i++){
           Z[1][i] = c2[i];//response->normMatrixSol.krTypes[j].fields[i].value;
           ZI[1] = ZI[1] + 1 - Z[1][i];
    }

	if(ZI[1]==0)
    {
         //String Err = "Значения одного и того же критерия во всех альтернативах не могут быть одинаковы";
         //System.out.println("Values of the same criteria in all alternatives cannot be identical.");       
         err = -1;
         goto labelErrCalc2;
    }

	Den[1] = 1/(ZI[1] * ZI[1]);
    Den1 += Den[1];

	// и , наконец, для разностей непрерывных и ступенчатых управлений
	for(i=0; i<count_alternatives; i++){
           Z[2][i] = c3[i];
           ZI[2] = ZI[2] + 1 - Z[2][i];
    }

	if(ZI[2]==0)
    {
         //String Err = "Значения одного и того же критерия во всех альтернативах не могут быть одинаковы";
         //System.out.println("Values of the same criteria in all alternatives cannot be identical.");       
         err = -1;
         goto labelErrCalc2;
    }

	Den[2] = 1/(ZI[2] * ZI[2]);
    Den1 += Den[2];

    /////////////////////////
    for (j = 0; j < nKrit; j++)
       W[j] = Den[j] / Den1;	   
	
    
	//fprintf(file, "calculated weights criterion\n");
	
    for (i = 0; i < count_alternatives; i++)
        F[i] = 0;
    for (i = 0; i < count_alternatives; i++) {
       double fKr[3];
       // для цены
       fKr[0] = (1 - c1[i]) * W[0];
       F[i] = F[i] + fKr[0];

	   // для модуля отклонения
	   fKr[1] = (1 - c2[i]) * W[1];
       F[i] = F[i] + fKr[1];

	   // для разности непрерывного и ступенчатого управления
	   fKr[2] = (1 - c3[i]) * W[2];
       F[i] = F[i] + fKr[2];

	   pAlternatives[i].target_func = F[i];
    }
    
	//fprintf(file, "calculated criteria function\n");

	// найдем альтернативу у которой величина целевой функции имеет минимальное значение
	// и установим признак isBest в состояние true
	iMin = -1;
	min = 1000000;	
	count_fact_alt = count_alternatives - 1;

    for (j = 0; j < count_fact_alt; j++) 
	{
       if (pAlternatives[j].target_func < min)
       {
		   min  = pAlternatives[j].target_func;     
		   iMin = j;
	   }
    }

	pAlternatives[iMin].is_best = true;

	/*for(i=0; i<count_alternatives; i++){
		if(fabs(pAlternatives[i].target_func - min) < 0.0001)
			pAlternatives[i].is_best = true;
		break;
	}*/

	if(iMin == -1)
		err = -1;

labelErrCalc2:
    // Освобождаем занятые ресурсы и возвращаяем результат выполнения функции
    free(c1);
    free(c2);
					
	free(W);
	free(Den);
	free(ZI);

	free(F);

    for(int i=0; i<nKrit; i++)
	    free(Z[i]);

	//fclose(file);
	return err;   
}

     
void minDob(int count_alternatives, LPAlternative pAlternatives, int typeKr, double* d) { 
                                                
    double minKr;

	if(typeKr == 1){ //Price
        for (int i = 0; i < count_alternatives; i++)
            d[i] = pAlternatives[i].price;
	}
	else if(typeKr == 2) // mod_otklon
	{
		for (int i = 0; i < count_alternatives; i++)
			d[i] = pAlternatives[i].mod_otklon;
	}else{ //  DeltaOG_ON
		for (int i = 0; i < count_alternatives; i++)
			d[i] = pAlternatives[i].deltaOG_ON;
	}

    minKr = d[0];
    for (int j = 1; j < count_alternatives; j++) {
       if (d[j] < minKr)
          minKr = d[j];     
    }
    
    for (int j = 0; j < count_alternatives; j++) {
       d[j] = minKr / d[j];   
    }
            
}


#pragma endregion