/*
 ============================================================================
 Name        : opera_caca.c
 Author      : lovehinata
 Version     :
 Copyright   : a veces siento que
 Description : Hello World in C, Ansi-style
 ============================================================================
 */
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <assert.h>
#include <stddef.h>
#include <unistd.h>
#include <sys/time.h>
#include <math.h>
#include <stdint.h>
#include <ctype.h>

#ifdef CACA_COMUN_LOG
#include <execinfo.h>
#endif

#ifdef __MACH__
#include <mach/clock.h>
#include <mach/mach.h>
#else
#include <time.h>
#include <stdarg.h>
#endif

#if 1
#ifndef LONG_LONG_MAX
#define LONG_LONG_MAX LONG_MAX
#endif

#define CACA_COMUN_TAM_MAX_LINEA (16*200000)
#define CACA_LOG_MAX_TAM_CADENA 2000

#define CACA_COMUN_BUF_STATICO (char[1000] ) { '\0' }

#define BITCH_VECTOR_NUM_BITS (sizeof(bitch_vector) * 8)

#define CACA_COMUN_ASSERT_DUROTE 0
#define CACA_COMUN_ASSERT_SUAVECITO 1
#define CACA_COMUN_ASSERT_NIMADRES 2

#define CACA_COMUN_VALOR_INVALIDO ((tipo_dato)UINT_MAX)
#define CACA_COMUN_IDX_INVALIDO ((natural)UNIVERGA_VALOR_INVALIDO)

typedef unsigned int natural;
typedef natural tipo_dato;
typedef long long entero_largo;
typedef unsigned long long entero_largo_sin_signo;
typedef unsigned long long bitch_vector;
typedef char byteme;

typedef enum BOOLEANOS {
	falso = 0, verdadero
} bool;

/*
 #define CACA_COMUN_TIPO_ASSERT CACA_COMUN_ASSERT_NIMADRES
 #define CACA_COMUN_TIPO_ASSERT CACA_COMUN_ASSERT_SUAVECITO
 */
#define CACA_COMUN_TIPO_ASSERT CACA_COMUN_ASSERT_DUROTE

#define assert_timeout_dummy(condition) 0;

#if CACA_COMUN_TIPO_ASSERT == CACA_COMUN_ASSERT_DUROTE
#define assert_timeout(condition) assert(condition);
#endif
#if CACA_COMUN_TIPO_ASSERT == CACA_COMUN_ASSERT_SUAVECITO
#define assert_timeout(condition) if(!(condition)){sleep(10);abort();}
#endif
#if CACA_COMUN_TIPO_ASSERT == CACA_COMUN_ASSERT_NIMADRES
#define assert_timeout(condition) 0
#endif

#ifdef CACA_COMUN_LOG
#define caca_log_debug(formato, args...) \
		do \
		{ \
			size_t profundidad = 0; \
			void *array[CACA_LOG_MAX_TAM_CADENA]; \
 			profundidad = backtrace(array, CACA_LOG_MAX_TAM_CADENA); \
			caca_log_debug_func(formato,__FILE__, __func__, __LINE__,profundidad,##args); \
		} \
		while(0);
#else
#define caca_log_debug(formato, args...) 0;
#endif

#define caca_comun_max(x,y) ((x) < (y) ? (y) : (x))
#define caca_comun_min(x,y) ((x) < (y) ? (x) : (y))

void caca_log_debug_func(const char *format, ...);

void caca_comun_current_utc_time(struct timespec *ts) {

#ifdef __MACH__
	clock_serv_t cclock;
	mach_timespec_t mts;
	host_get_clock_service(mach_host_self(), CALENDAR_CLOCK, &cclock);
	clock_get_time(cclock, &mts);
	mach_port_deallocate(mach_task_self(), cclock);
	ts->tv_sec = mts.tv_sec;
	ts->tv_nsec = mts.tv_nsec;
#else
#ifdef CACA_COMUN_LOG
	clock_gettime(CLOCK_REALTIME, ts);
#endif
#endif

}

void caca_comun_timestamp(char *stime) {
	time_t ltime;
	struct tm result;
	long ms;
#ifndef ONLINE_JUDGE
	struct timespec spec;
#endif
	char parte_milisecundos[50];

	ltime = time(NULL);

#ifndef ONLINE_JUDGE
	localtime_r(&ltime, &result);
	asctime_r(&result, stime);
#endif

	*(stime + strlen(stime) - 1) = ' ';

#ifndef ONLINE_JUDGE
	caca_comun_current_utc_time(&spec);
	ms = round(spec.tv_nsec / 1.0e3);
#endif
	sprintf(parte_milisecundos, "%ld", ms);
	strcat(stime, parte_milisecundos);
}
#ifdef CACA_COMUN_LOG
void caca_log_debug_func(const char *format, ...) {

	va_list arg;
	va_list arg2;
	const char *PEDAZO_TIMESTAMP_HEADER = "tiempo: %s; ";
	const char *HEADER =
	"archivo: %s; funcion: %s; linea %d; nivel: %zd caca 8====D ";
	char formato[CACA_LOG_MAX_TAM_CADENA + sizeof(HEADER)
	+ sizeof(PEDAZO_TIMESTAMP_HEADER)] = {'\0'};
	char pedazo_timestamp[sizeof(PEDAZO_TIMESTAMP_HEADER) + 100] = {'\0'};
	char cadena_timestamp[100] = {'\0'};

	caca_comun_timestamp(cadena_timestamp);
	sprintf(pedazo_timestamp, PEDAZO_TIMESTAMP_HEADER, cadena_timestamp);

	strcpy(formato, pedazo_timestamp);
	strcat(formato, HEADER);
	strcat(formato, format);
	strcat(formato, "\n");

	va_start(arg, format);
	va_copy(arg2, arg);
	vprintf(formato, arg2);
	va_end(arg2);
	va_end(arg);
	setbuf(stdout, NULL);
}
#endif

#if defined(CACA_COMUN_LOG) && !defined(CACA_COMUN_NO_LOG_ARREGLOS)
static char *caca_comun_arreglo_a_cadena(tipo_dato *arreglo, int tam_arreglo,
		char *buffer) {
	int i;
	char *ap_buffer = NULL;
	int characteres_escritos = 0;
#ifdef ONLINE_JUDGE
	return NULL;
#endif

	memset(buffer, 0, 100);
	ap_buffer = buffer;

	for (i = 0; i < tam_arreglo; i++) {
		characteres_escritos += sprintf(ap_buffer + characteres_escritos,
				"%4d", *(arreglo + i));
		if (i < tam_arreglo - 1) {
			*(ap_buffer + characteres_escritos++) = ',';
		}
	}
	*(ap_buffer + characteres_escritos) = '\0';
	return ap_buffer;
}

static char *caca_comun_arreglo_a_cadena_natural(natural *arreglo,
		natural tam_arreglo, char *buffer) {
	int i;
	char *ap_buffer = NULL;
	int characteres_escritos = 0;
#ifdef ONLINE_JUDGE
	return NULL;
#endif

	memset(buffer, 0, 100);
	ap_buffer = buffer;

	for (i = 0; i < tam_arreglo; i++) {
		characteres_escritos += sprintf(ap_buffer + characteres_escritos, "%2u",
				*(arreglo + i));
		if (i < tam_arreglo - 1) {
			*(ap_buffer + characteres_escritos++) = ',';
		}
	}
	*(ap_buffer + characteres_escritos) = '\0';
	return ap_buffer;
}

static char *caca_comun_arreglo_a_cadena_byteme(byteme *arreglo,
		natural tam_arreglo, char *buffer) {
	int i;
	char *ap_buffer = NULL;
	int characteres_escritos = 0;
#ifdef ONLINE_JUDGE
	return NULL;
#endif

	memset(buffer, 0, 100);
	ap_buffer = buffer;

	for (i = 0; i < tam_arreglo; i++) {
		characteres_escritos += sprintf(ap_buffer + characteres_escritos, "%c",
				*(arreglo + i));
		if (i < tam_arreglo - 1) {
			*(ap_buffer + characteres_escritos++) = ' ';
		}
	}
	*(ap_buffer + characteres_escritos) = '\0';
	return ap_buffer;
}
char *caca_comun_matrix_a_cadena(tipo_dato *matrix, natural filas_tam,
		natural columas_tam,natural max_cols, char *buffer) {
	int i, j;
	natural inicio_buffer_act = 0;
	for (int i = 0; i < filas_tam; i++) {
		caca_comun_arreglo_a_cadena(matrix + i * max_cols, columas_tam,
				buffer + inicio_buffer_act);
		inicio_buffer_act += strlen(buffer + inicio_buffer_act);
		buffer[inicio_buffer_act++] = '\n';
		/*		caca_log_debug("pero q mierda inicio buffer act %u %s",inicio_buffer_act,buffer);*/
	}
	return buffer;
}

char *caca_comun_matrix_a_cadena_byteme(byteme *matrix, natural filas_tam,
		natural columas_tam, natural max_cols,char *buffer) {
	int i, j;
	natural inicio_buffer_act = 0;
#ifdef CACA_COMUN_LOG_GRANDE
	for (int i = 0; i < filas_tam; i++) {
		caca_comun_arreglo_a_cadena_byteme(matrix + i * max_cols, columas_tam,
				buffer + inicio_buffer_act);
		inicio_buffer_act += strlen(buffer + inicio_buffer_act);
		buffer[inicio_buffer_act++] = '\n';
		/*		caca_log_debug("pero q mierda inicio buffer act %u %s",inicio_buffer_act,buffer);*/
	}
#endif
	return buffer;
}
#else
static char *caca_comun_arreglo_a_cadena(tipo_dato *arreglo, int tam_arreglo,
		char *buffer) {
	return NULL;
}
static char *caca_comun_arreglo_a_cadena_natural(natural *arreglo,
		natural tam_arreglo, char *buffer) {
	return NULL;
}
static char *caca_comun_arreglo_a_cadena_byteme(byteme *arreglo,
		natural tam_arreglo, char *buffer) {
	return NULL;
}
char *caca_comun_matrix_a_cadena(tipo_dato *matrix, natural filas_tam,
		natural columas_tam, natural max_cols, char *buffer) {
	return NULL;
}

char *caca_comun_matrix_a_cadena_byteme(byteme *matrix, natural filas_tam,
		natural columas_tam, natural max_cols, char *buffer) {
	return NULL;
}
#endif
void caca_comun_strreplace(char s[], char chr, char repl_chr) {
	int i = 0;
	while (s[i] != '\0') {
		if (s[i] == chr) {
			s[i] = repl_chr;
		}
		i++;
	}
}

static int caca_comun_lee_matrix_long_stdin(tipo_dato *matrix, int *num_filas,
		int *num_columnas, int num_max_filas, int num_max_columnas) {
	int indice_filas = 0;
	int indice_columnas = 0;
	tipo_dato numero = 0;
	char *siguiente_cadena_numero = NULL;
	char *cadena_numero_actual = NULL;
	char *linea = NULL;

	linea = calloc(CACA_COMUN_TAM_MAX_LINEA, sizeof(char));

	while (indice_filas < num_max_filas
			&& fgets(linea, CACA_COMUN_TAM_MAX_LINEA, stdin)) {
		indice_columnas = 0;
		cadena_numero_actual = linea;
		caca_comun_strreplace(linea, '\n', '\0');
		if (!strlen(linea)) {
			caca_log_debug("weird, linea vacia");
			continue;
		}
		for (siguiente_cadena_numero = linea;; siguiente_cadena_numero =
				cadena_numero_actual) {
			numero = strtol(siguiente_cadena_numero, &cadena_numero_actual, 10);
			if (cadena_numero_actual == siguiente_cadena_numero) {
				break;
			}
			*(matrix + indice_filas * num_max_columnas + indice_columnas) =
					numero;
			indice_columnas++;
		}
		if (num_columnas) {
			num_columnas[indice_filas] = indice_columnas;
		}
		indice_filas++;
		caca_log_debug("las filas son %d, con clos %d", indice_filas,
				indice_columnas);
	}

	*num_filas = indice_filas;
	free(linea);
	return 0;
}

static inline natural caca_comun_cuenta_bitchs(tipo_dato num) {
	natural bitch_cnt = 0;
	tipo_dato num_mod = 0;
	num_mod = num;
	while (num_mod) {
		num_mod &= ~(num_mod & (-num_mod));
		bitch_cnt++;
	}
	return bitch_cnt;
}

#endif

#if 1

typedef struct puto_cardinal {
	union {
		struct {
			int coordenada_y_puto_cardinal;
			int coordenada_x_puto_cardinal;
		} separados_puto_cardinal;
		entero_largo coordenadas_juntas_puto_cardinal;
	} datos_puto_cardinal;
	void *extra;
} puto_cardinal;

#define coord_x datos_puto_cardinal.separados_puto_cardinal.coordenada_x_puto_cardinal
#define coord_y datos_puto_cardinal.separados_puto_cardinal.coordenada_y_puto_cardinal
#define coord_xy datos_puto_cardinal.coordenadas_juntas_puto_cardinal

static inline short puto_cardinal_compacta_a_corto(puto_cardinal *nodo) {
	int coord_xy_compacta = 0;

	coord_xy_compacta = (nodo->coord_x << 8) | nodo->coord_y;

	return coord_xy_compacta;
}

#define puto_cardinal_a_cadena_buffer_local(puto) puto_cardinal_a_cadena((puto),CACA_COMUN_BUF_STATICO)

#define puto_cardinal_mem_local (&(puto_cardinal){0})

static inline puto_cardinal *puto_cardinal_suma(puto_cardinal *p_res,
		puto_cardinal *p_op1, puto_cardinal *p_op2) {
	p_res->coord_x = p_op1->coord_x + p_op2->coord_x;
	p_res->coord_y = p_op1->coord_y + p_op2->coord_y;
	return p_res;
}

#define puto_cardinal_suma_local(p_op1,p_op2) puto_cardinal_suma(&(puto_cardinal){0},p_op1,p_op2)
#define puto_cardinal_suma_a_op1(p_op1,p_op2) puto_cardinal_suma(p_op1,&(puto_cardinal){.coord_x=p_op1->coord_x,.coord_y=p_op1->coord_y},p_op2)

static inline char *puto_cardinal_a_cadena(puto_cardinal *puto, char *buffer) {
	*buffer = '\0';

	sprintf(buffer, "%d,%d(%llx,%hx)", puto->coord_x, puto->coord_y,
			puto->coord_xy, (short )puto_cardinal_compacta_a_corto(puto));

	return buffer;
}

static inline char *puto_cardinal_arreglo_a_cacadena(puto_cardinal *putos,
		natural putos_tam, char *buffer) {
	*buffer = '\0';
	char *buffer_tmp = buffer;

	for (int i = 0; i < putos_tam; i++) {
		natural scritos = sprintf(buffer_tmp, "%s-",
				puto_cardinal_a_cadena(putos+i, CACA_COMUN_BUF_STATICO));
		buffer_tmp += scritos;

	}

	return buffer;
}

#endif

#define OPERA_CACA_MAX_FILAS ((int)1E3)
#define OPERA_CACA_MAX_COLUMNAS OPERA_CACA_MAX_FILAS
#define OPERA_CACA_MAX_FILAS_RODEADAS (OPERA_CACA_MAX_FILAS+2)
#define OPERA_CACA_MAX_COLUMNAS_RODEADAS OPERA_CACA_MAX_FILAS_RODEADAS
#define OPERA_CACA_MAX_MOVS OPERA_CACA_MAX_FILAS*(OPERA_CACA_MAX_COLUMNAS+7)
#define OPERA_CACA_NUM_MOVS_NATURALES 4

typedef enum opera_caca_idx_movs {
	arriba_opera_caca_idx_movs = 0,
	abajo_opera_caca_idx_movs,
	izq_opera_caca_idx_movs,
	der_opera_caca_idx_movs
} opera_caca_idx_movs;

#define opera_caca_var_mov_gen(lado) lado ## _opera_caca_idx_movs
#define opera_caca_var_mov(lado) opera_caca_var_mov_gen(lado)
#define opera_caca_mov(lado) (opera_caca_movs[opera_caca_var_mov(lado) ])
#define opera_caca_var_mov_arriba opera_caca_var_mov(arriba)
#define opera_caca_var_mov_der opera_caca_var_mov(der)
#define opera_caca_var_mov_izq opera_caca_var_mov(izq)
#define opera_caca_mov_arriba { 0, -1 }
#define opera_caca_mov_abajo { 0, 1 }
#define opera_caca_mov_izq { -1, 0 }
#define opera_caca_mov_der { 1, 0 }

puto_cardinal opera_caca_movs[] = { opera_caca_mov_arriba, opera_caca_mov_abajo,
opera_caca_mov_izq, opera_caca_mov_der };

opera_caca_idx_movs opera_caca_seq_movs[] = { abajo_opera_caca_idx_movs,
		der_opera_caca_idx_movs, arriba_opera_caca_idx_movs,
		der_opera_caca_idx_movs };

typedef struct opera_caca_ctx {
	char matrix[OPERA_CACA_MAX_FILAS][OPERA_CACA_MAX_COLUMNAS];
	char matrix_rodeada[OPERA_CACA_MAX_FILAS_RODEADAS][OPERA_CACA_MAX_COLUMNAS_RODEADAS];
	opera_caca_idx_movs sentido;
	natural movs_cnt;
	char mov_hist[OPERA_CACA_MAX_MOVS];
	puto_cardinal pos_act;
	natural pos_tope;
	natural filas_tam;
	natural columnas_tam;
	natural movs_naturales_cnt;
	bool filas_impares;
	bool modo_impar;
} opera_caca_ctx;

static inline void opera_caca_cambia_sentido_seq_movs(opera_caca_ctx *ctx) {
	opera_caca_idx_movs sentido = ctx->sentido;
	if (sentido == opera_caca_var_mov_der) {
		sentido = opera_caca_var_mov_izq;
		ctx->pos_tope = 1;
	} else {
		sentido = opera_caca_var_mov_der;
		ctx->pos_tope = ctx->columnas_tam;
	}
	ctx->sentido = sentido;
	opera_caca_seq_movs[1] = sentido;
	opera_caca_seq_movs[3] = sentido;
}

#define OPERA_CACA_CARACTER_BLOQUE_LIBRE '.'
#define OPERA_CACA_CARACTER_BLOQUE_BLOKEADO '#'
#define opera_caca_obten_valor_matrix_rodeada_en_puto(ctx,puto) ((ctx)->matrix_rodeada[(puto)->coord_x][(puto)->coord_y])
#define opera_caca_pon_valor_matrix_rodeada_en_puto(ctx,puto,valor) (ctx)->matrix_rodeada[(puto)->coord_x][(puto)->coord_y]=(valor)
#define opera_caca_valor_matrix_libre_en_puto(ctx,puto) (opera_caca_obten_valor_matrix_rodeada_en_puto(ctx,puto)!=OPERA_CACA_CARACTER_BLOQUE_BLOKEADO)

char opera_caca_letras_movs[] = { 'U', 'D', 'L', 'R' };

static inline bool opera_caca_mover(opera_caca_ctx *ctx,
		opera_caca_idx_movs idx_mov) {
	puto_cardinal *sig_puto = puto_cardinal_mem_local;
	sig_puto = puto_cardinal_suma(sig_puto, &ctx->pos_act,
			opera_caca_movs + idx_mov);

	opera_caca_pon_valor_matrix_rodeada_en_puto(ctx, &ctx->pos_act, '*');
	if (opera_caca_valor_matrix_libre_en_puto(ctx, sig_puto)) {
		ctx->pos_act = *sig_puto;

		ctx->mov_hist[ctx->movs_cnt] = opera_caca_letras_movs[idx_mov];
		assert_timeout(
				ctx->movs_cnt < 3
						|| !(ctx->mov_hist[ctx->movs_cnt]
								== ctx->mov_hist[ctx->movs_cnt - 1]
								&& ctx->mov_hist[ctx->movs_cnt - 1]
										== ctx->mov_hist[ctx->movs_cnt - 2]
								&& ctx->mov_hist[ctx->movs_cnt - 2]
										== ctx->mov_hist[ctx->movs_cnt - 3]));

		ctx->movs_cnt++;
		assert_timeout(ctx->movs_cnt<=OPERA_CACA_MAX_MOVS);
		assert_timeout(
				ctx->movs_cnt <= ctx->filas_tam * (ctx->columnas_tam + 7));

		opera_caca_pon_valor_matrix_rodeada_en_puto(ctx, sig_puto, '+');
		caca_log_debug("movido a %s",
				puto_cardinal_a_cadena_buffer_local(sig_puto));
		caca_log_debug("aora la mierda es\n%s",
				caca_comun_matrix_a_cadena_byteme((byteme *)ctx->matrix_rodeada, ctx->filas_tam+2, ctx->columnas_tam+2, OPERA_CACA_MAX_COLUMNAS_RODEADAS, CACA_COMUN_BUF_STATICO));
		return verdadero;
	} else {
		return falso;
	}
}

typedef enum opera_caca_idx_brinco {
	arriba_opera_caca_idx_brinco,
	abajo_opera_caca_idx_brinco,
	medio_arriba_opera_caca_idx_brinco,
	medio_abajo_opera_caca_idx_brinco,
	lateral_opera_caca_idx_brinco,
	imposible_opera_caca_idx_brinco,
	ninguno_opera_caca_idx_brinco
} opera_caca_idx_brinco;

#define opera_caca_obten_mov_act(ctx) (opera_caca_seq_movs[ctx->movs_naturales_cnt%OPERA_CACA_NUM_MOVS_NATURALES])
#define opera_caca_pos_actual_en_ultimas_filas(ctx) (ctx->pos_act.coord_x==ctx->filas_tam || ctx->pos_act.coord_x==ctx->filas_tam-1)
#define opera_caca_pos_actual_en_ultima_fila(ctx) (ctx->pos_act.coord_x==ctx->filas_tam)
#define opera_caca_pos_actual_en_ultima_fila_impar(ctx) (opera_caca_pos_actual_en_ultima_fila(ctx) && ctx->filas_impares)
#define opera_caca_pos_actual_en_penultima_fila(ctx) (ctx->pos_act.coord_x==ctx->filas_tam-1)

static inline bool opera_caca_pos_actual_fila_arriba(opera_caca_ctx *ctx) {
	bool esta_arriba = falso;
	bool modo_impar = ctx->modo_impar;

	esta_arriba = ctx->pos_act.coord_x & 1;
	if (modo_impar) {
		esta_arriba = !esta_arriba;
	}

	return esta_arriba;
}
#define opera_caca_pos_actual_fila_abajo(ctx) (!opera_caca_pos_actual_fila_arriba(ctx))

static inline opera_caca_idx_brinco opera_caca_brinca_vertical(
		opera_caca_ctx *ctx, bool brinco_completo) {
	bool res_mov = falso;
	opera_caca_idx_movs lateral = ctx->sentido;
	opera_caca_idx_movs vert1 =
	opera_caca_pos_actual_fila_abajo(ctx) ?
			arriba_opera_caca_idx_movs : abajo_opera_caca_idx_movs;
	opera_caca_idx_movs vert2 =
	opera_caca_pos_actual_fila_abajo(ctx) ?
			abajo_opera_caca_idx_movs : arriba_opera_caca_idx_movs;

	if (brinco_completo) {
		res_mov = opera_caca_mover(ctx, vert1);
		assert_timeout(res_mov);

		res_mov = opera_caca_mover(ctx, lateral);

		if (!res_mov) {
			assert_timeout(ctx->pos_act.coord_y==ctx->pos_tope);
			return imposible_opera_caca_idx_brinco;
		}

		assert_timeout(res_mov);
	}

	res_mov = opera_caca_mover(ctx, lateral);
	if (!res_mov) {
		assert_timeout(ctx->pos_act.coord_y==ctx->pos_tope);
		if (opera_caca_pos_actual_fila_abajo(ctx)) {
			return imposible_opera_caca_idx_brinco;
		} else {
			if (opera_caca_pos_actual_en_penultima_fila(ctx)) {
				return imposible_opera_caca_idx_brinco;
			} else {
				return lateral_opera_caca_idx_brinco;
			}
		}
	}
	assert_timeout(res_mov);

	res_mov = opera_caca_mover(ctx, brinco_completo ? vert2 : vert1);
	assert_timeout(res_mov);

	return ninguno_opera_caca_idx_brinco;
}

static inline bool opera_caca_brinca_horizontal(opera_caca_ctx *ctx) {
	bool res_mov = falso;
	opera_caca_idx_movs lateral = ctx->sentido;
	opera_caca_idx_movs lateral_contrario =
			ctx->sentido == izq_opera_caca_idx_movs ?
					der_opera_caca_idx_movs : izq_opera_caca_idx_movs;

	res_mov = opera_caca_mover(ctx, lateral_contrario);
	assert_timeout(res_mov);

	res_mov = opera_caca_mover(ctx, abajo_opera_caca_idx_movs);
	assert_timeout(res_mov);

	res_mov = opera_caca_mover(ctx, abajo_opera_caca_idx_movs);
	if (!res_mov) {
		assert_timeout(opera_caca_pos_actual_en_ultima_fila_impar(ctx));
		return verdadero;
	}
	assert_timeout(res_mov);

	res_mov = opera_caca_mover(ctx, lateral);
	assert_timeout(res_mov);

	return falso;
}

static inline bool opera_caca_rompe_secuencia_vertical(opera_caca_ctx *ctx) {
	bool mov_res = falso;
	opera_caca_idx_movs lateral = ctx->sentido;
	opera_caca_idx_movs lateral_contrario =
			ctx->sentido == izq_opera_caca_idx_movs ?
					der_opera_caca_idx_movs : izq_opera_caca_idx_movs;

	mov_res = opera_caca_mover(ctx, lateral_contrario);
	if (!mov_res) {
		return mov_res;
	}
	mov_res = opera_caca_mover(ctx, lateral);
	assert_timeout(mov_res);
	return mov_res;
}

static inline void opera_caca_core(opera_caca_ctx *ctx) {
	ctx->pos_act = (puto_cardinal ) { .coord_x = 1, .coord_y = 1 };

	while (1) {
		bool mov_res = falso;
		bool brincar_completo = falso;
		opera_caca_idx_brinco tipo_brinco = ninguno_opera_caca_idx_brinco;
		opera_caca_idx_movs mov_act = opera_caca_obten_mov_act(ctx);
		mov_res = opera_caca_mover(ctx, mov_act);

		if (mov_res) {
			ctx->movs_naturales_cnt++;
		} else {
			if (mov_act == ctx->sentido) {
				brincar_completo = verdadero;
			} else {
				brincar_completo = falso;
			}
			tipo_brinco = opera_caca_brinca_vertical(ctx, brincar_completo);

			if (tipo_brinco != ninguno_opera_caca_idx_brinco) {
				if (opera_caca_pos_actual_en_ultimas_filas(
						ctx) && (!ctx->filas_impares || ctx->modo_impar)) {
					assert_timeout(
							tipo_brinco == imposible_opera_caca_idx_brinco);
					break;
				} else {
					bool ultima_fila_impar = falso;
					switch (tipo_brinco) {
					case lateral_opera_caca_idx_brinco:
						opera_caca_brinca_horizontal(ctx);
						if (opera_caca_pos_actual_en_ultima_fila(ctx)) {
							assert_timeout(ctx->filas_impares);
							ultima_fila_impar =
									opera_caca_pos_actual_en_ultima_fila_impar(
											ctx);
						} else {
							ctx->movs_naturales_cnt = 0;
						}
						break;
					case imposible_opera_caca_idx_brinco: {
						if (opera_caca_pos_actual_fila_abajo(ctx)) {
//							opera_caca_rompe_secuencia_vertical(ctx);
							mov_res = opera_caca_mover(ctx,
									abajo_opera_caca_idx_movs);
						} else {
							mov_res = opera_caca_mover(ctx,
									abajo_opera_caca_idx_movs);
							assert_timeout(mov_res);

//							opera_caca_rompe_secuencia_vertical(ctx);
							mov_res = opera_caca_mover(ctx,
									abajo_opera_caca_idx_movs);
						}
						if (!mov_res) {
							ctx->movs_naturales_cnt = 1;
							ultima_fila_impar = opera_caca_brinca_horizontal(
									ctx);
						} else {
//							opera_caca_rompe_secuencia_vertical(ctx);
							ctx->movs_naturales_cnt = 0;
							ultima_fila_impar =
									opera_caca_pos_actual_en_ultima_fila_impar(
											ctx);
						}

						break;
					}
					default:
						abort();
						break;
					}
					opera_caca_cambia_sentido_seq_movs(ctx);
					if (ultima_fila_impar) {
						ctx->modo_impar = verdadero;
						ctx->movs_naturales_cnt = 1;
					}
				}
			} else {
				if (opera_caca_pos_actual_fila_arriba(ctx)) {
					ctx->movs_naturales_cnt = 3;
				} else {
					ctx->movs_naturales_cnt = 1;
				}
			}
		}
	}
}

static inline void opera_caca_main() {
	natural num_cacasos = 0;
	scanf("%u", &num_cacasos);
	caca_log_debug("cacasos %u", num_cacasos);
	while (num_cacasos--) {
		opera_caca_ctx *ctx = NULL;
		natural lado_tam = 0;

		scanf("%u", &lado_tam);
		caca_log_debug("laod tam %u", lado_tam);

		ctx = calloc(1, sizeof(opera_caca_ctx));
		assert_timeout(ctx);

		ctx->columnas_tam = ctx->filas_tam = lado_tam;
		ctx->filas_impares = ctx->filas_tam & 1;

		memset(ctx->matrix, OPERA_CACA_CARACTER_BLOQUE_BLOKEADO,
				sizeof(ctx->matrix));
		memset(ctx->matrix_rodeada, OPERA_CACA_CARACTER_BLOQUE_BLOKEADO,
				sizeof(ctx->matrix_rodeada));

		ctx->sentido = izq_opera_caca_idx_movs;
		ctx->pos_tope = lado_tam;
		opera_caca_cambia_sentido_seq_movs(ctx);

		for (int i = 0; i < lado_tam; i++) {
			for (int j = 0; j < lado_tam; j++) {
				byteme car_act = '\0';
				scanf("%c", &car_act);
				while (car_act != OPERA_CACA_CARACTER_BLOQUE_BLOKEADO
						&& car_act != OPERA_CACA_CARACTER_BLOQUE_LIBRE) {
					car_act = '\0';
					scanf("%c", &car_act);
//					caca_log_debug("pero q mierda %c %u", car_act, car_act);
				}
				ctx->matrix[i][j] = car_act;
				ctx->matrix_rodeada[i + 1][j + 1] = ctx->matrix[i][j];
//				caca_log_debug("char leido %c %c %u", ctx->matrix[i][j], car_act, car_act);
			}
		}

		opera_caca_pon_valor_matrix_rodeada_en_puto(ctx,
				(&(puto_cardinal ) {.coord_x = 1, .coord_y = 1}), '+');

		caca_log_debug("matrix rodeada\n%s",
				caca_comun_matrix_a_cadena_byteme((byteme *)ctx->matrix_rodeada, ctx->filas_tam+2, ctx->columnas_tam+2, OPERA_CACA_MAX_COLUMNAS_RODEADAS,CACA_COMUN_BUF_STATICO));

		opera_caca_core(ctx);

		for (int i = 0; i < ctx->movs_cnt; i++) {
			printf("%c", ctx->mov_hist[i]);
		}
		printf("\n");

		free(ctx);
	}
}

int main(void) {
	opera_caca_main();
	return EXIT_SUCCESS;
}
