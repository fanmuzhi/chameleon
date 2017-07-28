

#include "basetypes.h"
typedef struct sts_bin_list_elem_s
{
    uint8_t    sts_bit;   /* Status Bit */
    uint8_t    bin;       /* Bin Code */
}sts_bin_list_elem_t;
#define STS_BIT_LIST_END                      255

/*!
*******************************************************************************
** Check the layer id's of the viper asic against a built-in list
**
** @param[in]       *p_status_words      Pointer to start of status words
** @param[in]       *p_info              Pointer to start of status bit, bin
**                                         code list.  Status bit = 255 
**                                         indicates the end of the list
** @param[out]      *p_bin_codes         Pointer to start of the bin_codes
** @param[in/out]   *p_nbins             In - Pointer to size of the bin
**                                         code list available.
**                                       Out - Number of bin codes
**
**  Entries for mask layers are included in the built-in list through the use 
**  of compile directives 
*/

void get_bin_fail_list(  uint32_t  *p_status_words, 
        const sts_bin_list_elem_t  *p_info, 
                          uint8_t  *p_bin_codes,
                          uint8_t  *p_nbins);


