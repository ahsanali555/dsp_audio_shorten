#ifndef RICE_CODEC_H_
#define RICE_CODEC_H_

long rice_encoder(int *data, unsigned int *stream_out, int n_data, int k_rice);
void rice_decoder(unsigned int *stream_in, int *data, int n_data, int k_rice);
unsigned int rice_parameter(int *data, int n_data);

#endif /* RICE_CODEC_H_ */
