#ifndef PREDICTION_H_
#define PREDICTION_H_

void prediction_error(int *data, int *p_filt, int *pstate, int *error, short n_data, short n_p);
void prediction_error_P1(int *data, int *pstate, int *error, unsigned short n_data);
void prediction_error_P2(int *data, int *pstate, int *error, unsigned short n_data);

void reconstruct(int *error, int *p_filt, int *pstate, int *data, unsigned short n_data, unsigned short n_p);
void reconstruct_P1(int *error, int *pstate, int *data, unsigned short n_data);
void reconstruct_P2(int *error, int *pstate, int *data, unsigned short n_data);

#endif /* PREDICTION_H_ */
