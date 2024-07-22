/* compute prediction error with n_p-order filter
 * data: pointer to data buffer
 * p_filt: pointer to filter coefficients (Q3.12)
 * pstate: pointer to buffer for predictor state (size = n_p+1)
 * error: pointer to output buffer (same size as data)
 * n_data: data buffer length
 * n_p: predictor order */

void prediction_error(int *data, int *p_filt, int *pstate, int *error, unsigned short n_data, unsigned short n_p)
{
	int i,j,k,P;
		long acc;

		// get circular pointer stored in extra buffer location
		P = pstate[n_p];
		for (i = 0; i < n_data; i++)
		{
			// compute prediction, initialize accumulator with first tap product
			acc = (long)pstate[P] * p_filt[0];
			// cycle with circular pointer: from P+1 to end of buffer
			for (j = 1, k = P+1; k < n_p; j++,k++)
			{
				acc += (long)pstate[k] * p_filt[j];
			}
			// from start of buffer to P-1
			for (k = 0; k < P; j++,k++)
			{
				acc += (long)pstate[k] * p_filt[j];
			}
			// scale for Q3.12
			acc = (acc + 0x800) >> 12;
			// clip to 16 bit signed
			if (acc > 32767) acc = 32767;
			if (acc < -32768) acc = -32768;
			// reconstruct sample
			error[i]= data[i] - (int)acc;
			// update circular pointer
			if (--P < 0) P = n_p - 1;
			// load new sample in circular buffer
			pstate[P] = data[i];
		}
		// store circular pointer in extra buffer location
		pstate[n_p] = P;
}

/* Pn: compute prediction error using polynomial prediction filter of order n
 * data: pointer to data buffer
 * pstate: pointer to buffer for predictor state (size = n)
 * error: pointer to output buffer (same size as data)
 * n_data: data buffer length
 */

void prediction_error_P1(int *data, int *pstate, int *error, unsigned short n_data)
{
	int i;

	for (i = 0; i < n_data; i++)
	{
		// reconstruct sample
		error[i] = data[i] - pstate[0];
		// load new sample
		pstate[0] = data[i];
	}
}

void prediction_error_P2(int *data, int *pstate, int *error, unsigned short n_data)
{
	int i;
	long acc;

	for (i = 0; i < n_data; i++)
	{
		// compute prediction
		acc = (long)pstate[0] * 2;
		acc += (long)pstate[1] * (-1);
		// clip to 16 bit signed
		if (acc > 32767) acc = 32767;
		if (acc < -32768) acc = -32768;
		// reconstruct sample
		error[i] = data[i] - (int)acc;
		// shift delay line
		pstate[1] = pstate[0];
		// load new sample
		pstate[0] = data[i];
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

/*
 * reconstruct samples from prediction error with n_p-order filter
 * error: pointer to output buffer (same size as data)
 * p_filt: pointer to filter coefficients (Q3.12)
 * pstate: pointer to buffer for predictor state (size = n_p+1)
 * data: pointer to data buffer
 * error: pointer to output buffer (same size as data)
 * n_data: data buffer length
 * n_p: predictor order
 *
 */

void reconstruct(int *error, int *p_filt, int *pstate, int *data, unsigned short n_data, unsigned short n_p)
{
	int i,j,k,P;
	long acc;

	// get circular pointer stored in extra buffer location
	P = pstate[n_p];
	for (i = 0; i < n_data; i++)
	{
		// compute prediction, initialize accumulator with first tap product
		acc = (long)pstate[P] * p_filt[0];
		// cycle with circular pointer: from P+1 to end of buffer
		for (j = 1, k = P+1; k < n_p; j++,k++)
		{
			acc += (long)pstate[k] * p_filt[j];
		}
		// from start of buffer to P-1
		for (k = 0; k < P; j++,k++)
		{
			acc += (long)pstate[k] * p_filt[j];
		}
		// scale for Q3.12
		acc = (acc + 0x800) >> 12;
		// clip to 16 bit signed
		if (acc > 32767) acc = 32767;
		if (acc < -32768) acc = -32768;
		// reconstruct sample
		data[i] = error[i] + (int)acc;
		// update circular pointer
		if (--P < 0) P = n_p - 1;
		// load new sample in circular buffer
		pstate[P] = data[i];
	}
	// store circular pointer in extra buffer location
	pstate[n_p] = P;
}

/*
 * Pn: reconstruct sample from pred. error using polynomial prediction filter of order n
 * error: pointer to output buffer (same size as data)
 * pstate: pointer to buffer for predictor state (size = n)
 * data: pointer to data buffer
 * n_data: data buffer length
 *
 */

void reconstruct_P1(int *error, int *pstate, int *data, unsigned short n_data)
{
	int i;

	for (i = 0; i < n_data; i++)
	{
		// reconstruct sample
		data[i] = error[i] + pstate[0];
		// load new sample
		pstate[0] = data[i];
	}
}
void reconstruct_P2(int *error, int *pstate, int *data, unsigned short n_data)
{
	int i;
	long acc;

	for (i = 0; i < n_data; i++)
	{
		// compute prediction
		acc = (long)pstate[0] * 2;
		acc += (long)pstate[1] * (-1);
		// clip to 16 bit signed
		if (acc > 32767) acc = 32767;
		if (acc < -32768) acc = -32768;
		// reconstruct sample
		data[i] = error[i] + (int)acc;
		// shift delay line
		pstate[1] = pstate[0];
		// load new sample
		pstate[0] = data[i];
	}
}
