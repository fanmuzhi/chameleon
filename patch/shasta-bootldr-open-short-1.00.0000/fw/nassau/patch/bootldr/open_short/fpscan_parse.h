

#include "basetypes.h"


typedef struct reg_check_fail_s
{
    uint32_t  address;
    uint32_t  expected_val;
    uint32_t  read_val;
}reg_check_fail_t;
void get_reg_check_fail_list(reg_check_fail_t **pp_list, uint32_t *p_n_list);
void copy_reg_check_vail_list(uint32_t *p_list);


void register_bulk_write(uint32_t start_address, const uint32_t *data, uint32_t nwords);
void register_bulk_set(uint32_t start_address, uint32_t data, uint32_t nwords);



/*!
*******************************************************************************
** Description
**
** @param[in]       *tagp             
** @param[in]       nbytes     
** @param[in]       dm_only  
** @param[out]      *p_n_tags_used      
** @param[out]      *p_n_tags_skip       
**
** Return FW Code 
**   0 = OK
**   Anything else is an error 
*/

uint32_t fpscan_tagsparse(const uint32_t   *tagp, 
                                uint32_t   nbytes, 
                                  bool_t   dm_only, 
                                uint32_t   *p_n_tags_used, 
                                uint32_t   *p_n_tags_skip);



uint32_t parse_iotas(uint32_t iota_addr, 
                     uint16_t iota_type,
                     uint32_t *p_n_tags_used, 
                     uint32_t *p_n_tags_skip);



#define FIND_IOTA_FAIL_ADDRESS      0  /* Return value if IOTA tag is not found */
uint32_t find_iota_addr(uint32_t iota_start, uint32_t *p_match_data, uint32_t match_len, bool_t b_skip_hdr_cmp);



void get_iota_tag_list(uint32_t iota_start, uint32_t *p_data, uint32_t *p_n_data);
