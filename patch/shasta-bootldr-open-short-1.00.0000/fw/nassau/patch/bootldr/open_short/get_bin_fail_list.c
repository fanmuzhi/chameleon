/*
 * This unpublished code is Copyright (c) 2006-2015, Synaptics Inc.
 * All rights reserved.
 */

/* get_bin_fail_list.c

 */

#include "get_bin_fail_list.h"

void get_bin_fail_list(  uint32_t  *p_status_words, 
        const sts_bin_list_elem_t  *p_info, 
                          uint8_t  *p_bin_codes,
                          uint8_t  *p_nbins)
{
    uint8_t   i;
    uint8_t   j;
    uint8_t   qnty;
    uint32_t  mask;

    for(i=0; i<*p_nbins; i++)
    {
        p_bin_codes[i] = 0;
    }

    qnty = 0;
    i = 0;
    while ((STS_BIT_LIST_END != p_info[i].sts_bit) && (0 < *p_nbins))
    {
        mask = (1 << (p_info[i].sts_bit%32));
        if (0 < (p_status_words[p_info[i].sts_bit/32] & mask))
        {
            /* Check to see if bin code is already in list */
            for(j=0; j<qnty; j++)
            {
                if (p_bin_codes[j] == p_info[i].bin) break;

            }
            if (j == qnty)
            {
                p_bin_codes[qnty] = p_info[i].bin;
                *p_nbins = *p_nbins - 1;
                qnty = qnty + 1;
            }
        }
        i = i + 1;
    }
    *p_nbins = qnty; 
}
 





