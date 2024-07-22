#include "p_filter.h"
#include "prediction.h"
#include "rice_codec.h"
#include "samples2.h"

#define BUFF_LEN	256
#define K_RICE		9

// allocate buffers
int buffer_enc[P_LEN+1];
int buffer_dec[P_LEN+1];
int error_enc[BUFF_LEN];
int error_dec[BUFF_LEN];
int output[BUFF_LEN];

// allocate large code buffer
unsigned int code[BUFF_LEN*8];

// compute total squared error between sequences s1 and s2
long compute_error(int* s1, int *s2, int len)
{
	int i;
	long acc = 0, tmp;
	for (i = 0; i < len; i++)
	{
		tmp = (long)s1[i] - s2[i];
		acc += tmp*tmp;
	}
	return acc;
}

long num_bits, rec_err;

//////////////////////////////////////////////////////////////

int main(void) {

	long i;

	// zero-out buffers
	for (i = 0; i < P_LEN+1; i++)
	{
		buffer_enc[i] = 0;
		buffer_dec[i] = 0;
	}

	for (i = 0; i < BUFF_LEN*8; i++)
	{
		code[i] = 0;
	}
	num_bits = 0;
	rec_err = 0;


	// write here code to process samples from &samples[i*BUFF_LEN] to &samples[i*BUFF_LEN + BUFF_LEN - 1]	
	for (i = 0; i < DATA_LEN/BUFF_LEN; i++)
	{		
		// proceed to alternatively test the prediction_error, prediction_errorP1, and prediction_errorP2 functions 
		// after you wrote the code inside their definition in prediction.c

		// the first test to perform to be sure that you wrote a correct prediction_error function is:
		// comment out the rice encoder / decoder functions here below and see if rec_err = 0
		// then you can proceed to write the rice encoder

		prediction_error(&samples[i*BUFF_LEN], p_filt, buffer_enc, error_enc, BUFF_LEN, 5);
		num_bits += rice_encoder(error_enc, code, BUFF_LEN, K_RICE);
		rice_decoder(code, error_dec, BUFF_LEN, K_RICE);
		reconstruct(error_dec, p_filt, buffer_dec, output, BUFF_LEN, 5);

		//prediction_error_P2(&samples[i*BUFF_LEN], buffer_enc, error_enc, BUFF_LEN);
		//num_bits += rice_encoder(error_enc, code, BUFF_LEN, K_RICE);
		//rice_decoder(code, error_dec, BUFF_LEN, K_RICE);
		//reconstruct_P2(error_dec, buffer_dec, output, BUFF_LEN);

		//prediction_error_P2(&samples[i*BUFF_LEN], buffer_enc, error_enc, BUFF_LEN);
		//num_bits += rice_encoder(error_enc, code, BUFF_LEN, K_RICE);
		//rice_decoder(code, error_dec, BUFF_LEN, K_RICE);
		//reconstruct_P2(error_dec, buffer_dec, output, BUFF_LEN);

		// verify reconstruction error (should be zero for lossless compression)
		rec_err += compute_error(&samples[i*BUFF_LEN], output, BUFF_LEN);
	}

	return 0;
}
