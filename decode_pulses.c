/***********************************************************************
Copyright (c) 2006-2011, Skype Limited. All rights reserved.
Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:
- Redistributions of source code must retain the above copyright notice,
this list of conditions and the following disclaimer.
- Redistributions in binary form must reproduce the above copyright
notice, this list of conditions and the following disclaimer in the
documentation and/or other materials provided with the distribution.
- Neither the name of Internet Society, IETF or IETF Trust, nor the
names of specific contributors, may be used to endorse or promote
products derived from this software without specific prior written
permission.
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.
***********************************************************************/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "main.h"

/*********************************************/
/* Decode quantization indices of excitation */
/*********************************************/
void silk_decode_pulses(
    ec_dec                      *psRangeDec,                    /* I/O  Compressor data structure                   */
    opus_int16                  pulses[],                       /* O    Excitation signal                           */
    const opus_int              signalType,                     /* I    Sigtype                                     */
    const opus_int              quantOffsetType,                /* I    quantOffsetType                             */
    const opus_int              frame_length                    /* I    Frame length                                */
)
{
    opus_int   i, j, k, iter, abs_q, nLS,ii,trash;
	opus_int   RateLevelIndex = 0;
    opus_int   sum_pulses[ MAX_NB_SHELL_BLOCKS ], nLshifts[ MAX_NB_SHELL_BLOCKS ];
    opus_int16 *pulses_ptr;
    const opus_uint8 *cdf_ptr;
	int RLI = 0;
	int sig = 0;
	FILE *s_pulse;
	FILE *rate_level;
	//FILE *p0 = fopen("F://opus//opus数据库//opus数据库//NN//DataSet1//pred//p0.txt","r");
	FILE *temp;
	FILE *NLs;
	/*FILE *pred_pulse;*/
	//FILE *temp1 = fopen("F:/opus/opus数据库/opus数据库/NN/Last_attempt/A11_216/temp1.txt", "w+");
	//FILE *sig_type;
	opus_int16       *q_ptr;
	extern  FILE *pred_pulse;

    /*********************/
    /* Decode rate level */
    /*********************/
	sig = signalType;
    RateLevelIndex = ec_dec_icdf( psRangeDec, silk_rate_levels_iCDF[ signalType >> 1 ], 8 );
	//rate_level = fopen("F:/opus/opus数据库/opus数据库/NN/DataSet1/pred/one_seed/A11_217/ratelevel.txt", "a+");
	//fprintf(rate_level, "%d\n", RateLevelIndex);
	//fclose(rate_level);

	//sig_type = fopen("F:/opus/opus数据库/opus数据库/NN/signaltype_val.txt", "a+");
	//fprintf(sig_type,"%d\n", sig);
	//fclose(sig_type);
    /* Calculate number of shell blocks */
    silk_assert( 1 << LOG2_SHELL_CODEC_FRAME_LENGTH == SHELL_CODEC_FRAME_LENGTH );
    iter = silk_RSHIFT( frame_length, LOG2_SHELL_CODEC_FRAME_LENGTH );
    if( iter * SHELL_CODEC_FRAME_LENGTH < frame_length ) {
        celt_assert( frame_length == 12 * 10 ); /* Make sure only happens for 10 ms @ 12 kHz */
        iter++;
    }

    /***************************************************/
    /* Sum-Weighted-Pulses Decoding                    */
    /***************************************************/
    cdf_ptr = silk_pulses_per_block_iCDF[ RateLevelIndex ];
    for( i = 0; i < iter; i++ ) {
        nLshifts[ i ] = 0;
        sum_pulses[ i ] = ec_dec_icdf( psRangeDec, cdf_ptr, 8 );

        /* LSB indication */
        while( sum_pulses[ i ] == SILK_MAX_PULSES + 1 ) {
            nLshifts[ i ]++;
            /* When we've already got 10 LSBs, we shift the table to not allow (SILK_MAX_PULSES + 1) */
            sum_pulses[ i ] = ec_dec_icdf( psRangeDec,
                    silk_pulses_per_block_iCDF[ N_RATE_LEVELS - 1] + ( nLshifts[ i ] == 10 ), 8 );
        }

		//s_pulse = fopen("F:/opus/opus数据库/opus数据库/NN/Last_Attempt//A11_217//36k//sum_pulse.txt", "a+");
		//NLs = fopen("F://opus//opus数据库//opus数据库//NN//DataSet1/pred/one_seed/A11_217/A11_217/NLS.txt", "a+");
		//fprintf(s_pulse, "%d ",sum_pulses[i]);
		//fprintf(NLs, "%d ", nLshifts[i]);
		/*if (i > iter-2) {
			fprintf(s_pulse, "\n");
		}
		fclose(s_pulse);*/
		//fclose(NLs);
    }

    /***************************************************/
    /* Shell decoding                                  */
    /***************************************************/
    for( i = 0; i < iter; i++ ) {
        if( sum_pulses[ i ] > 0 ) {
            silk_shell_decoder( &pulses[ silk_SMULBB( i, SHELL_CODEC_FRAME_LENGTH ) ], psRangeDec, sum_pulses[ i ] );
			/*for (k = 0; k < SHELL_CODEC_FRAME_LENGTH; k++) {
				pulses_ptr = &pulses[silk_SMULBB(i, SHELL_CODEC_FRAME_LENGTH)];
				//fscanf(p0, "%d", &trash);
				//pulses_ptr[k] = trash;
			}*/
        } else {
            silk_memset( &pulses[ silk_SMULBB( i, SHELL_CODEC_FRAME_LENGTH ) ], 0, SHELL_CODEC_FRAME_LENGTH * sizeof( pulses[0] ) );
			/*for (k = 0; k < SHELL_CODEC_FRAME_LENGTH; k++) {
				fscanf(p0, "%d", &trash);
			}*/
		}
    }
	/*while (EOF!=fscanf(p0, "%d", &trash)) {
		fprintf(temp, "%d\n", trash);
	}	*/
	//fclose(p0);
	//fclose(temp);
	//remove("F:/opus/opus数据库/opus数据库/NN/DataSet1/pred/p0.txt");
	//rename("F:/opus/opus数据库/opus数据库/NN/DataSet1/pred/temp.txt", "F:/opus/opus数据库/opus数据库/NN/DataSet1/pred/p0.txt");
	
    /***************************************************/
    /* LSB Decoding                                    */
    /***************************************************/
    for( i = 0; i < iter; i++ ) {
        if( nLshifts[ i ] > 0 ) {
            nLS = nLshifts[ i ];
            pulses_ptr = &pulses[ silk_SMULBB( i, SHELL_CODEC_FRAME_LENGTH ) ];
            for( k = 0; k < SHELL_CODEC_FRAME_LENGTH; k++ ) {
                abs_q = pulses_ptr[ k ];
                for( j = 0; j < nLS; j++ ) {
                    abs_q = silk_LSHIFT( abs_q, 1 );
					trash = ec_dec_icdf(psRangeDec, silk_lsb_iCDF, 8);
					//temp = fopen("F://opus//opus数据库//opus数据库//NN//DataSet1/pred/one_seed/A11_217/A11_217/temp.txt", "a+");
					//fprintf(temp, "%d ", trash);
					//fclose(temp);
                    abs_q += trash;
                }
                pulses_ptr[ k ] = abs_q;
            }
            /* Mark the number of pulses non-zero for sign decoding. */
            sum_pulses[ i ] |= nLS << 5;

		}
    }

	
    /****************************************/
    /* Decode and add signs to pulse signal */
    /****************************************/
    silk_decode_signs( psRangeDec, pulses, frame_length, signalType, quantOffsetType, sum_pulses );
	/*将预测合成的脉冲替代原始脉冲*/
	q_ptr = pulses;
	for (i = 0; i < iter; i++) {
		for (k = 0; k < SHELL_CODEC_FRAME_LENGTH; k++) {
			fscanf(pred_pulse, "%d", &trash);
			q_ptr[k] = trash;
		}
		q_ptr += SHELL_CODEC_FRAME_LENGTH;

	}

	/*pred_pulse = fopen("F:\\opus\\opus数据库\\opus数据库\\NN\\Last_attempt\\A11_216\\pred_pulse.txt", "r");
	q_ptr = pulses;
	for (i = 0; i < iter; i++) {
		for (k = 0; k < SHELL_CODEC_FRAME_LENGTH; k++) {
			fscanf(pred_pulse, "%d", &trash);
			q_ptr[k] = trash;
			printf("%d ", trash);
			printf("%d ", q_ptr[k]);
			printf("%d\n", i*k);
		}
		q_ptr += SHELL_CODEC_FRAME_LENGTH;

	}
	while (EOF!=fscanf(pred_pulse, "%d", &trash)) {
		fprintf(temp1, "%d\n", trash);
	}
	fclose(temp1);
	fclose(pred_pulse);
	temp1 = fopen("F:/opus/opus数据库/opus数据库/NN/Last_attempt/A11_216/temp1.txt", "r");
	pred_pulse = fopen("F:\\opus\\opus数据库\\opus数据库\\NN\\Last_attempt\\A11_216\\pred_pulse.txt", "w+");
	while (EOF != fscanf(temp1, "%d", &trash)) {
		fprintf(pred_pulse, "%d\n", trash);
	}
	fclose(temp1);
	fclose(pred_pulse);*/
}
